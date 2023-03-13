#pragma once

#include <vulkan/vulkan.h>

namespace GE
{
	class CommandBuffer
	{
	public:

		CommandBuffer(uint32_t count, VkCommandBufferUsageFlags usage = 0);
		~CommandBuffer();

		VkCommandBuffer Begin(int index);

		void End();

		void Free();

		void Submit(int index = 0);

		VkCommandBuffer GetBuffer(int index = 0);

		VkCommandBuffer GetCurrent() const { return m_CurrentCommandBuffer; }

		std::vector<VkCommandBuffer> GetBuffers() const { return m_CommandBuffers; }

		uint32_t GetCount() { return m_Count; }

		operator VkCommandBuffer&()
		{
			return m_CurrentCommandBuffer;
		}

	private:

		VkCommandBuffer m_CurrentCommandBuffer = VK_NULL_HANDLE;

		std::vector<VkCommandBuffer> m_CommandBuffers;

		Ref<class Device> m_Device = nullptr;

		uint32_t m_Count = 0;

		VkCommandBufferUsageFlags m_Usage;
	};
}