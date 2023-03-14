#pragma once

#include "Renderer/Shader.h"
#include <vulkan/vulkan.h>

namespace GE
{
	enum class ShaderStage : uint8_t
	{
		None = 0,
		Vertex = 1,
		Fragment = 2
	};

	class VulkanShader : public Shader
	{
	public:

		VulkanShader(const std::string& filename);

		virtual ~VulkanShader();

		std::vector<VkShaderModule> GetShaderModules() { return m_ShaderModules; }

	protected:

		std::string ReadFile(const std::string& filename);

		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderStage, std::string>& shaderSources);

		std::unordered_map<ShaderStage, std::string> PreProcess(const std::string& source);

		VkShaderModule CreateShaderModule(uint32_t* code, size_t size);

	private:
		std::unordered_map<ShaderStage, std::vector<uint32_t>> m_Spirv;

		std::vector<VkShaderModule> m_ShaderModules;

		std::string m_FilePath;

		Ref<class Device> m_Device;
	};
}