#pragma once

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

namespace GE
{
	struct Material
	{
		glm::vec3 m_DiffuseColor = glm::vec3(.5f, .5f, .5f);
	};

	struct FSimplePushConstantData
	{
		glm::mat4 m_Model;

		Material m_Material;
	};


	class SimpleRenderSystem
	{
	public:
		const int MAX_FRAMES_IN_FLIGHT = 2;

		SimpleRenderSystem(VkRenderPass renderpass, VkDescriptorSetLayout setLayout);

		~SimpleRenderSystem();

		class GraphicsPipeline* GetPipeline() const { return m_GraphicsPipeline.get(); }

		VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

	private:
		void CreatePipelineLayout(VkDescriptorSetLayout setLayout);

		void CreatePipeline(VkRenderPass renderpass);


	private:

		Ref<class Device> m_Device;

		Scope<class GraphicsPipeline> m_GraphicsPipeline;

		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
	};
}