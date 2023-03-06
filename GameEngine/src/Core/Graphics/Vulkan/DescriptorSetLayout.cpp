#include "pch.h"
#include "DescriptorSetLayout.h"
#include "Device.h"

DescriptorSetLayout::DescriptorSetLayout(Device* device, uint32_t binding)
	:m_Device(device)
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = binding;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	if (vkCreateDescriptorSetLayout(m_Device->GetDevice(), &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout!");
	}
}

DescriptorSetLayout::~DescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(m_Device->GetDevice(), m_DescriptorSetLayout, nullptr);
}
