#pragma once

#include "Device.h"

namespace GE
{
	struct FSimplePushConstantData
	{
		glm::mat4 m_Model;
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

		Ref<Device> m_Device;

		Scope<class GraphicsPipeline> m_GraphicsPipeline;

		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
	};
}