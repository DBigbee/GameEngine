#include "pch.h"
#include "Descriptors.h"
#include "Device.h"
#include "VulkanRendererAPI.h"
#include "VulkanRenderCommand.h"

namespace GE
{
	DescriptorSetLayout::DescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings)
		:m_Bindings(bindings)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings{};
		for (auto& kv : bindings)
		{
			layoutBindings.push_back(kv.second);
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutInfo.pBindings = layoutBindings.data();


		if (vkCreateDescriptorSetLayout(*m_Device, &layoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor set layout!");
		}
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(*m_Device, m_DescriptorSetLayout, nullptr);
	}

	DescriptorSetLayout::Builder& DescriptorSetLayout::Builder::AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stateFlags, uint32_t count)
	{
		GE_CORE_ASSERT(m_Bindings.count(binding) == 0 , "Binding already in use!");

		VkDescriptorSetLayoutBinding layoutBinding{};
		layoutBinding.binding = binding;
		layoutBinding.descriptorType = descriptorType;
		layoutBinding.descriptorCount = count;
		layoutBinding.stageFlags = stateFlags;
		layoutBinding.pImmutableSamplers = nullptr;
		m_Bindings[binding] = layoutBinding;
		return *this;
	}

	DescriptorPool::Builder& DescriptorPool::Builder::AddPoolSize(VkDescriptorType descriptorType, uint32_t count)
	{
		// TODO: insert return statement here
		m_PoolSizes.push_back({ descriptorType, count });
		return *this;
	}

	DescriptorPool::Builder& DescriptorPool::Builder::SetPoolFlags(VkDescriptorPoolCreateFlags poolFlags)
	{
		// TODO: insert return statement here
		m_PoolFlags = poolFlags;
		return *this;
	}

	DescriptorPool::Builder& DescriptorPool::Builder::SetMaxSets(uint32_t count)
	{
		// TODO: insert return statement here
		m_MaxSets = count;
		return *this;
	}

	Scope<DescriptorPool> DescriptorPool::Builder::Build() const
	{
		return MakeScope<DescriptorPool>(m_MaxSets, m_PoolFlags, m_PoolSizes);
	}

	Scope<DescriptorSetLayout> DescriptorSetLayout::Builder::Build() const
	{
		return MakeScope<DescriptorSetLayout>(m_Bindings);
	}

	DescriptorPool::DescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes)
	{
		m_Device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice();

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = maxSets;
		poolInfo.flags = poolFlags;

		auto device = VulkanRenderCommand::GetVulkanRenderAPI()->GetDevice()->GetDevice();
		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create descriptor pool!");
		}
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(*m_Device, m_DescriptorPool, nullptr);
	}


	bool DescriptorPool::AllocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &descriptorSetLayout;

		if (vkAllocateDescriptorSets(*m_Device, &allocInfo, &descriptor) != VK_SUCCESS)
		{
			return false;
		}

		return true;
	}

	void DescriptorPool::FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const
	{
		vkFreeDescriptorSets(*m_Device, m_DescriptorPool, static_cast<uint32_t>(descriptors.size()), descriptors.data());
	}

	void DescriptorPool::ResetPool()
	{
		vkResetDescriptorPool(*m_Device, m_DescriptorPool, 0);
	}

	DescriptorWriter::DescriptorWriter(DescriptorSetLayout* setLayout, DescriptorPool* pool)
		:m_SetLayout(setLayout), m_Pool(pool)
	{
	}

	DescriptorWriter& DescriptorWriter::WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo)
	{
		// TODO: insert return statement here
		GE_CORE_ASSERT(m_SetLayout->m_Bindings.count(binding) == 1 , "Layout does not contain specified binding!");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		GE_CORE_ASSERT(bindingDescription.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple.");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.dstBinding = binding;
		//write.dstSet = &m_SetLayout->GetSetLayout();
		write.dstArrayElement = 0;
		write.descriptorType = bindingDescription.descriptorType;
		write.descriptorCount = 1;
		write.pBufferInfo = bufferInfo;
		write.pTexelBufferView = nullptr;

		m_Writes.push_back(write);
		return *this;
	}

	DescriptorWriter& DescriptorWriter::WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo)
	{
		// TODO: insert return statement here
		GE_CORE_ASSERT(m_SetLayout->m_Bindings.count(binding) == 1 , "Layout does not contain specified binding!");

		auto& bindingDescription = m_SetLayout->m_Bindings[binding];

		GE_CORE_ASSERT(bindingDescription.descriptorCount == 1 , "Binding single descriptor info, but binding expects multiple.");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.dstBinding = binding;
		write.dstArrayElement = 0;
		write.descriptorType = bindingDescription.descriptorType;
		write.descriptorCount = 1;
		write.pImageInfo = imageInfo;
		write.pTexelBufferView = nullptr;

		m_Writes.push_back(write);
		return *this;
	}

	bool DescriptorWriter::Build(VkDescriptorSet& set)
	{
		bool result = m_Pool->AllocateDescriptorSet(m_SetLayout->GetSetLayout(), set);
		if (!result)
		{
			return false;
		}

		OverWrite(set);
		return true;
	}

	void DescriptorWriter::OverWrite(VkDescriptorSet& set)
	{
		for (auto& write : m_Writes)
		{
			write.dstSet = set;
		}

		vkUpdateDescriptorSets(*m_Pool->m_Device, static_cast<uint32_t>(m_Writes.size()), m_Writes.data(), 0, nullptr);
	}
}