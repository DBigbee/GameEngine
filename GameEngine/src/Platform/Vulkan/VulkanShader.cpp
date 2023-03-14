#include "pch.h"
#include "VulkanShader.h"
#include "Device.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"
#include "shaderc/shaderc.hpp"
#include <filesystem>

namespace GE
{
	namespace Utils
	{
		ShaderStage GetShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return ShaderStage::Vertex;
			if (type == "fragment" || type == "pixel")
				return ShaderStage::Fragment;

			assert(false && "Unknown shader type!");
			return ShaderStage(0);
		}

		shaderc_shader_kind ConvertShaderStageToShadercKind(ShaderStage stage)
		{
			switch (stage)
			{
			case GE::ShaderStage::Vertex:
				return shaderc_shader_kind::shaderc_glsl_vertex_shader;
			case GE::ShaderStage::Fragment:
				return shaderc_shader_kind::shaderc_glsl_fragment_shader;
			default:
				break;
			}

			assert(false);

			return shaderc_shader_kind(0);
		}

		std::string GLShaderStageCachedVulkanFileExtension(ShaderStage stage)
		{
			switch (stage)
			{
			case GE::ShaderStage::Vertex:
				return ".spriv.vert";
			case GE::ShaderStage::Fragment:
				return ".spirv.frag";
			default:
				break;
			}

			assert(false);
			return "";
		}

		const char* GetCacheDirectory()
		{
			return "assets/shaders";
		}
	}

	VulkanShader::VulkanShader(const std::string& filename)
		: m_FilePath(filename)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		auto source = ReadFile(filename);
		auto shaderSources = PreProcess(source);

		CompileOrGetVulkanBinaries(shaderSources);
	}

	VulkanShader::~VulkanShader()
	{
		
		for (auto shadermodule : m_ShaderModules)
		{
			vkDestroyShaderModule(*m_Device, shadermodule, nullptr);
		}
	}

	std::string VulkanShader::ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to open file!");
		}

		std::string results;
		size_t fileSize = (size_t)file.tellg();
		if (fileSize != -1)
		{
			results.resize(fileSize);
			file.seekg(0);
			file.read(results.data(), fileSize);
			file.close();
		}
		else
		{
			throw std::runtime_error("Could not read from file");
		}

		return results;
	}

	void VulkanShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderStage, std::string>& shaderSources)
	{
		shaderc::Compiler complier;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = false;
		if (optimize)
		{
			options.SetOptimizationLevel(shaderc_optimization_level_performance);
		}

		auto& shaderStages = m_Spirv;
		shaderStages.clear();

		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.stem().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);

			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderStages[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = complier.CompileGlslToSpv(source, Utils::ConvertShaderStageToShadercKind(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					throw std::runtime_error(module.GetErrorMessage());
				}

				shaderStages[stage] = std::vector<uint32_t>(module.begin(), module.end());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderStages[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}

				
			}

		}

		
		for (auto&& [stage, source] : shaderStages)
		{
			m_ShaderModules.push_back(CreateShaderModule(source.data(), source.size() * sizeof(uint32_t)));
		}

		m_Spirv = shaderStages;
	}

	std::unordered_map<ShaderStage, std::string> VulkanShader::PreProcess(const std::string& source)
	{
		std::unordered_map<ShaderStage, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			assert(eol != std::string::npos && "Syntax Error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			assert((uint8_t)Utils::GetShaderTypeFromString(type) && "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			assert(nextLinePos != std::string::npos && "Syntax Error");
			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::GetShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	VkShaderModule VulkanShader::CreateShaderModule(uint32_t* code, size_t size)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = size;
		createInfo.pCode = code;
		createInfo.pNext = nullptr;

		VkShaderModule shadeModule;
		if (vkCreateShaderModule(*m_Device, &createInfo, nullptr, &shadeModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module!");
		}

		return shadeModule;
	}
}