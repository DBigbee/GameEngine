#pragma once

#include <vulkan/vulkan.h>

class DescriptorSetLayout
{
public:

	class Builder
	{
	public:

		Builder(class Device* device) : m_Device(device) {};

		Builder& AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stateFlags, uint32_t count = 1);

		std::unique_ptr<DescriptorSetLayout> Build() const;

	private:

		class Device* m_Device = nullptr;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
	};

	DescriptorSetLayout(class Device* device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

	virtual ~DescriptorSetLayout();

	VkDescriptorSetLayout& GetSetLayout() { return m_DescriptorSetLayout; }

private:

	class Device* m_Device = nullptr;

	VkDescriptorSetLayout m_DescriptorSetLayout = VK_NULL_HANDLE;

	std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};

	friend class DescriptorWriter;
};

class DescriptorPool
{
public:

	class Builder
	{
	public:
		Builder(class Device* device) : m_Device(device) {};

		Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
		Builder& SetPoolFlags(VkDescriptorPoolCreateFlags poolFlags);
		Builder& SetMaxSets(uint32_t count);
		std::unique_ptr<DescriptorPool> Build() const;

	private:
		class Device* m_Device = nullptr;

		std::vector<VkDescriptorPoolSize> m_PoolSizes{};

		uint32_t m_MaxSets = 1000;

		VkDescriptorPoolCreateFlags m_PoolFlags = 0;
	};

	DescriptorPool(class Device* device, uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
	virtual ~DescriptorPool();

	bool AllocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

	void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

	void ResetPool();

private:

	class Device* m_Device = nullptr;

	VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;

	friend class DescriptorWriter;
};

class DescriptorWriter
{
public:

	DescriptorWriter(DescriptorSetLayout* setLayout, DescriptorPool* pool);

	DescriptorWriter& WriteBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
	DescriptorWriter& WriteImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

	bool Build(VkDescriptorSet& set);
	void OverWrite(VkDescriptorSet& set);

private:

	DescriptorSetLayout* m_SetLayout;
	DescriptorPool* m_Pool;
	std::vector<VkWriteDescriptorSet> m_Writes;
};