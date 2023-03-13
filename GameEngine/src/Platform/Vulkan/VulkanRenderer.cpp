#include "pch.h"
#include "VulkanRenderer.h"
#include "Renderer/RenderCommand.h"
#include "VulkanRenderCommand.h"
#include "GraphicsPipeline.h"
#include "Descriptors.h"
#include "Renderer/UniformBuffer.h"
#include "VulkanRendererAPI.h"
#include "Renderer/Model.h"
#include "Renderer/Mesh.h"
#include "SwapChain.h"
#include "VulkanUniformBuffer.h"
#include "Application.h"
#include "WinWindow.h"
#include "Renderer/Texture.h"
#include "VulkanTexture2D.h"

namespace GE
{
	struct FUniformBufferObject
	{
		glm::mat4 m_Model;

		glm::mat4 m_View;

		glm::mat4 m_Projection;
	};


	struct RendererData
	{
		Ref<class SimpleRenderSystem> m_RenderSystem;

		std::vector<Ref<class UniformBuffer>> m_UniformBuffers;

		Scope<class DescriptorSetLayout> m_DescriptorSetLayout{};

		std::vector<VkDescriptorSet> m_DescriptorSets;

		Ref<class DescriptorPool> m_DescriptorPool{};

		Ref<class SwapChain> m_SwapChain;

		bool m_IsFrameStarted = false;

		uint32_t m_CurrentFrameIndex = 0;

		FUniformBufferObject m_UniformBufferObject;

		Ref<Texture2D> m_Texture;
	};

	static RendererData s_Data;

	VulkanRenderer::VulkanRenderer()
	{
		
	}

	VulkanRenderer::~VulkanRenderer()
	{

	}

	void VulkanRenderer::BeginScene(const Camera* camera, const Transform& transform)
	{
		if (auto cmd = BeginFrame())
		{
			BeginRenderPass(cmd);

			////update uniform buffer
			s_Data.m_UniformBufferObject.m_View = glm::lookAt(transform.m_Position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			s_Data.m_UniformBufferObject.m_Projection = camera->GetProjection();

			//invert for vulkan
			s_Data.m_UniformBufferObject.m_Projection[1][1] *= -1;

			s_Data.m_UniformBuffers[s_Data.m_SwapChain->GetCurrentFrame()]->SetData((void*)&s_Data.m_UniformBufferObject, sizeof(FUniformBufferObject));

			vkCmdBindDescriptorSets(*s_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s_Data.m_RenderSystem->GetPipelineLayout(),
				0, 1, &s_Data.m_DescriptorSets[s_Data.m_SwapChain->GetCurrentFrame()], 0, nullptr);

			s_Data.m_RenderSystem->GetPipeline()->Bind(*s_CommandBuffer);
		}
	}

	void VulkanRenderer::EndScene()
	{
		if (*s_CommandBuffer)
		{
			EndRenderPass(*s_CommandBuffer);

			EndFrame();
		}	
	}

	void VulkanRenderer::DrawModel(const glm::mat4 transform, ModelComponent& src)
	{
		FSimplePushConstantData pushData{};
		pushData.m_Model = transform;

		//s_Data.m_UniformBufferObject.m_Model = transform;
		vkCmdPushConstants(*s_CommandBuffer, s_Data.m_RenderSystem->GetPipelineLayout(), VK_SHADER_STAGE_VERTEX_BIT |
			VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(FSimplePushConstantData), &pushData);

		for (auto mesh : src.m_Model->GetMeshes())
		{
			RenderCommand::DrawIndexed(mesh->GetVertexArray());
		}
	}

	void VulkanRenderer::Init()
	{
		s_Data.m_SwapChain = MakeScope<SwapChain>(VkExtent2D{800, 600});

		s_Data.m_UniformBuffers.resize((size_t)SwapChain::MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++)
		{
			s_Data.m_UniformBuffers[i] = UniformBuffer::Create(sizeof(FUniformBufferObject), 1);
		}

		s_Data.m_DescriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

		s_Data.m_Texture = Texture2D::Create("assets/textures/statue-g2725c3c85_1920.jpg");

		s_Data.m_DescriptorPool = DescriptorPool::Builder()
			.SetMaxSets(2)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
			.AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT)
			.Build();

		s_Data.m_DescriptorSetLayout = DescriptorSetLayout::Builder()
			.AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
			.Build();


		for (int i = 0; i < s_Data.m_DescriptorSets.size(); i++)
		{
			auto imageInfo = Cast<VulkanTexture2D>(s_Data.m_Texture.get())->GetImageInfo();
			auto bufferInfo = Cast<VulkanUniformBuffer>(s_Data.m_UniformBuffers[i].get())->GetBufferInfo();

			DescriptorWriter(s_Data.m_DescriptorSetLayout.get(), s_Data.m_DescriptorPool.get())
				.WriteBuffer(0, &bufferInfo)
				.WriteImage(1, &imageInfo)
				.Build(s_Data.m_DescriptorSets[i]);
		}

		s_Data.m_RenderSystem = MakeScope<SimpleRenderSystem>(s_Data.m_SwapChain->GetRenderPass(),
			s_Data.m_DescriptorSetLayout->GetSetLayout());
	}

	void VulkanRenderer::Shutdown()
	{
		auto device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();
		vkDeviceWaitIdle(*device);
	}

	void VulkanRenderer::OnWindowResized()
	{
		auto& device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();
		auto spec = Application::Get().GetWindow().GetSpecification();

		while (spec.m_Width == 0 || spec.m_Height == 0)
		{
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device->GetDevice());

		VkExtent2D extent = { spec.m_Width, spec.m_Height };
		if (s_Data.m_SwapChain == nullptr)
		{

			s_Data.m_SwapChain = MakeScope<SwapChain>(extent);
		}
		else
		{
			s_Data.m_SwapChain = MakeScope<SwapChain>(extent, std::move(s_Data.m_SwapChain));
			if (s_Data.m_SwapChain->GetImageCount() != s_CommandBuffer->GetCount())
			{
				s_CommandBuffer->Free();
				s_CommandBuffer = VulkanRenderCommand::GetVulkanRenderAPI()->CreateCommandBuffer();
			}
		}
	}

	VkCommandBuffer VulkanRenderer::BeginFrame()
	{
		assert(!s_Data.m_IsFrameStarted && "Cant call begin frame while already in progress!");

		VkResult result = s_Data.m_SwapChain->AquireNextImage(&s_Data.m_CurrentFrameIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			OnWindowResized();
			return nullptr;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed tp acquire swap chain image!");
		}
		s_Data.m_IsFrameStarted = true;


		s_CommandBuffer = VulkanRenderCommand::GetVulkanRenderAPI()->GetCommandBuffer();
		s_CommandBuffer->Begin(s_Data.m_SwapChain->GetCurrentFrame());

		//std::cout << "Began recording buffer at index " << s_Data.m_SwapChain->GetCurrentFrame() << std::endl;
		
		return *s_CommandBuffer;
	}

	void VulkanRenderer::EndFrame()
	{
		assert(s_Data.m_IsFrameStarted && "Cant call end frame while already in progress!");

		s_CommandBuffer->End();

		//std::cout << "end recording buffer at index " << s_Data.m_SwapChain->GetCurrentFrame() << std::endl;

		auto& window = Application::Get().GetWindow();
		auto result = s_Data.m_SwapChain->SubmitCommandBuffers(s_CommandBuffer->GetBuffers().data(), &s_Data.m_CurrentFrameIndex);

		//std::cout << "submitted buffers at index " << s_Data.m_CurrentFrameIndex << std::endl;

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowResized())
		{
			window.ResetWindowResizedFlag();
			OnWindowResized();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		s_Data.m_IsFrameStarted = false;

	
	}

	void VulkanRenderer::BeginRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(s_Data.m_IsFrameStarted && "Cant call begin swap render pass while already in progress!");
		assert(commandBuffer == *s_CommandBuffer && "Cant begin render on a different frame!");

		VkRenderPassBeginInfo renderpassInfo{};
		renderpassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderpassInfo.renderPass = s_Data.m_SwapChain->GetRenderPass();
		renderpassInfo.framebuffer = s_Data.m_SwapChain->GetFrameBuffers()[s_Data.m_CurrentFrameIndex];

		renderpassInfo.renderArea.offset = { 0 ,0 };
		renderpassInfo.renderArea.extent = s_Data.m_SwapChain->GetExtents();

		std::array<VkClearValue, 2> clearColors{};
		clearColors[0].color = { {0.1f, 0.1f, 0.1f, 1.0f} };
		clearColors[1].depthStencil = { 1.0f, 0 };
		renderpassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
		renderpassInfo.pClearValues = clearColors.data();

		vkCmdBeginRenderPass(commandBuffer, &renderpassInfo, VK_SUBPASS_CONTENTS_INLINE);



		int width, height = 0;
		auto window = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
		glfwGetWindowSize(window, &width, &height);
		
		VkExtent2D extents = { (uint32_t)width, (uint32_t)height };
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)extents.width;
		viewport.height = (float)extents.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0 ,0 };
		scissor.extent = extents;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


	}

	void VulkanRenderer::EndRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(s_Data.m_IsFrameStarted && "Cant call end swap render pass while already in progress!");
		assert(commandBuffer == *s_CommandBuffer && "Cant end render pass on a different frame!");

		vkCmdEndRenderPass(commandBuffer);
	}

	Ref<CommandBuffer> VulkanRenderer::s_CommandBuffer;
}



