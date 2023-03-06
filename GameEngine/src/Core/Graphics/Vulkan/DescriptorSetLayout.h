#pragma once

#include <vulkan/vulkan.h>

class DescriptorSetLayout
{
public:

	DescriptorSetLayout(class Device* device, uint32_t binding);

	virtual ~DescriptorSetLayout();

	VkDescriptorSetLayout& GetSetLayout() { return m_DescriptorSetLayout; }

private:

	class Device* m_Device;

	VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;
};