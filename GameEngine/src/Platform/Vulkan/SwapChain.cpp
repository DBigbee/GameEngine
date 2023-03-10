#include "pch.h"
#include "SwapChain.h"
#include "Image.h"
#include "ImageView.h"
#include "VulkanRenderCommand.h"
#include "VulkanRendererAPI.h"

namespace GE
{
	SwapChain::SwapChain(VkExtent2D extent)
		:m_Extent(extent)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		Init();
	}

	SwapChain::SwapChain(VkExtent2D extent, std::shared_ptr<class SwapChain> previous)
		:m_Extent(extent), m_OldSwapChain(previous)
	{

		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		Init();

		//clean up old swap chain
		m_OldSwapChain = nullptr;
	}

	void SwapChain::Init()
	{
		CreateSwapChain();
		CreateColorResources();
		CreateDepthResources();
		CreateImageViews();
		CreateRenderPass();
		CreateFramebuffers();
		CreateSyncObjects();
	}

	SwapChain::~SwapChain()
	{
		CleanUp();
	}

	VkResult SwapChain::AquireNextImage(uint32_t* imageIndex)
	{
		vkWaitForFences(m_Device->GetDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		vkResetFences(m_Device->GetDevice(), 1, &m_InFlightFences[m_CurrentFrame]);

		
		auto result =  vkAcquireNextImageKHR(m_Device->GetDevice(), m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, imageIndex);


		return result;
	}

	void SwapChain::ResetFence()
	{

	}

	VkResult SwapChain::SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex)
	{

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &buffers[m_CurrentFrame];


		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = imageIndex;
		presentInfo.pResults = nullptr; // Optional

		auto result = vkQueuePresentKHR(m_Device->GetPresentQueue(), &presentInfo);


		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		return result;
	}


	void SwapChain::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = m_Device->QuerySwapChainSupport(m_Device->GetPhysicalDevice());

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.m_Formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.m_PresentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.m_Capabilities);

		uint32_t imageCount = swapChainSupport.m_Capabilities.minImageCount + 1;

		if (swapChainSupport.m_Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.m_Capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.m_Capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Device->GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilityIndices indices = m_Device->FindQueueFamilies(m_Device->GetPhysicalDevice());
		uint32_t queueFamilyIndices[] = { indices.m_GraphicsFamily.value(), indices.m_PresentFamily.value() };

		if (indices.m_GraphicsFamily != indices.m_PresentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.pQueueFamilyIndices = nullptr; //optional
			createInfo.queueFamilyIndexCount = 0; //optional
		}

		createInfo.preTransform = swapChainSupport.m_Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = m_OldSwapChain == nullptr ? VK_NULL_HANDLE : m_OldSwapChain->GetSwapChain();

		if (vkCreateSwapchainKHR(m_Device->GetDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &imageCount, m_Images.data());

		m_ImageFormat = surfaceFormat.format;
		m_Extent = extent;
	}

	void SwapChain::CreateImageViews()
	{
		m_ImageViews.resize(m_Images.size());

		for (size_t i = 0; i < m_Images.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];

			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_ImageFormat;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_Device->GetDevice(), &createInfo, nullptr, &m_ImageViews[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create image views!");
			}
		}
	}

	void SwapChain::CreateRenderPass()
	{
		auto sampleCount = m_Device->GetSampleCount();
		VkAttachmentDescription depthAttachment{};
		depthAttachment.format = m_Device->FindDepthFormat();
		depthAttachment.samples = sampleCount;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef{};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = m_ImageFormat;
		colorAttachment.samples = sampleCount;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //store to memory to be read later

		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentDescription colorAttachmentResolve{};
		colorAttachmentResolve.format = m_ImageFormat;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentResolveRef{};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		//Subpass
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pResolveAttachments = &colorAttachmentResolveRef;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::vector<VkAttachmentDescription> attachments{ colorAttachment, depthAttachment, colorAttachmentResolve };
		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(m_Device->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Render Pass!");
		}
	}

	void SwapChain::CreateFramebuffers()
	{
		m_Framebuffers.resize(m_ImageViews.size());

		for (size_t i = 0; i < m_ImageViews.size(); i++)
		{
			std::array<VkImageView, 3> attachments = { m_ColorImageView->GetImageView(),  m_DepthImageView->GetImageView(), m_ImageViews[i] };

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			createInfo.renderPass = m_RenderPass;
			createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			createInfo.pAttachments = attachments.data();
			createInfo.width = m_Extent.width;
			createInfo.height = m_Extent.height;
			createInfo.layers = 1;

			if (vkCreateFramebuffer(m_Device->GetDevice(), &createInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create framebuffer!");
			}
		}


	}

	void SwapChain::CleanUp()
	{
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(m_Device->GetDevice(), m_ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(m_Device->GetDevice(), m_RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(m_Device->GetDevice(), m_InFlightFences[i], nullptr);
		}


		for (auto framebuffer : m_Framebuffers)
		{
			vkDestroyFramebuffer(m_Device->GetDevice(), framebuffer, nullptr);
		}


		for (auto imageView : m_ImageViews)
		{
			vkDestroyImageView(m_Device->GetDevice(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device->GetDevice(), m_SwapChain, nullptr);

		vkDestroyRenderPass(m_Device->GetDevice(), m_RenderPass, nullptr);


	}


	VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			VkExtent2D actualExtent =
			{
				static_cast<uint32_t>(m_Extent.width),
				static_cast<uint32_t>(m_Extent.height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void SwapChain::CreateSyncObjects()
	{
		m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(m_Device->GetDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to create semaphores!");
			}
		}
	}

	void SwapChain::CreateDepthResources()
	{
		VkSampleCountFlagBits samples = m_Device->GetSampleCount();
		VkFormat depthFormat = m_Device->FindDepthFormat();
		m_DepthImage = MakeScope<Image>(m_Extent.width, m_Extent.height, ImageProperties { depthFormat,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT , 1, samples});
		m_DepthImage->BindToMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_DepthImageView = MakeScope<ImageView>(m_DepthImage->GetImage(), ImageViewProperties{depthFormat, 1, VK_IMAGE_ASPECT_DEPTH_BIT});

		m_DepthImage->TransitionImageLayout(depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
	}

	void SwapChain::CreateColorResources()
	{
		VkSampleCountFlagBits samples = m_Device->GetSampleCount();
		VkFormat format = m_ImageFormat;

		m_ColorImage = MakeScope<Image>(m_Extent.width, m_Extent.height, ImageProperties{format,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 1, samples});
		m_ColorImage->BindToMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		m_ColorImageView = MakeScope<ImageView>(m_ColorImage->GetImage(), ImageViewProperties{ format, 1, VK_IMAGE_ASPECT_COLOR_BIT });
	}
}