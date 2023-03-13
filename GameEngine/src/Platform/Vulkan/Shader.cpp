#include "pch.h"
#include "Shader.h"
#include "Device.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{

	Shader::Shader(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		auto vertShaderCode = ReadFile(vertFilePath);
		auto fragShaderCode = ReadFile(fragFilePath);

		m_ShaderModules.resize(2);

		m_ShaderModules[0] = CreateShaderModule(vertShaderCode);
		m_ShaderModules[1] = CreateShaderModule(fragShaderCode);
	}

	Shader::~Shader()
	{
		
		for (auto shadermodule : m_ShaderModules)
		{
			vkDestroyShaderModule(*m_Device, shadermodule, nullptr);
		}
	}

	std::string Shader::ReadFile(const std::string& filename)
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

	VkShaderModule Shader::CreateShaderModule(const std::string& code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		createInfo.pNext = nullptr;

		VkShaderModule shadeModule;
		if (vkCreateShaderModule(*m_Device, &createInfo, nullptr, &shadeModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module!");
		}

		return shadeModule;
	}
}