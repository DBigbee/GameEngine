#pragma once

#include <vulkan/vulkan.h>

namespace GE
{
	class DescriptorSetLayout
	{
	public:

		class Builder
		{
		public:

			Builder() = default;

			Builder& AddBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stateFlags, uint32_t count = 1);

			Scope<DescriptorSetLayout> Build() const;

		private:

			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> m_Bindings{};
		};

		DescriptorSetLayout(std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);

		virtual ~DescriptorSetLayout();

		VkDescriptorSetLayout& GetSetLayout() { return m_DescriptorSetLayout; }

	private:

		Ref<class Device> m_Device;

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
			Builder() = default;

			Builder& AddPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& SetPoolFlags(VkDescriptorPoolCreateFlags poolFlags);
			Builder& SetMaxSets(uint32_t count);
			Scope<DescriptorPool> Build() const;

		private:

			std::vector<VkDescriptorPoolSize> m_PoolSizes{};

			uint32_t m_MaxSets = 1000;

			VkDescriptorPoolCreateFlags m_PoolFlags = 0;
		};

		DescriptorPool(uint32_t maxSets, VkDescriptorPoolCreateFlags poolFlags, const std::vector<VkDescriptorPoolSize>& poolSizes);
		virtual ~DescriptorPool();

		bool AllocateDescriptorSet(const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void FreeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

		void ResetPool();

	private:

		Ref<class Device> m_Device;

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
}