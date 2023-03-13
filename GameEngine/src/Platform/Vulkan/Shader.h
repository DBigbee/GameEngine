#pragma once

#include <vulkan/vulkan.h>

namespace GE
{
	class Shader
	{
	public:

		Shader(const std::string& vertFilePath, const std::string& fragFilePath);

		virtual ~Shader();

		std::vector<VkShaderModule> GetShaderModules() { return m_ShaderModules; }

	protected:

		std::string ReadFile(const std::string& filename);

		VkShaderModule CreateShaderModule(const std::string& code);

	private:
		std::vector<VkShaderModule> m_ShaderModules;

		std::string m_FilePaths[2];

		Ref<class Device> m_Device;
	};
}