#pragma once

namespace GE
{
	class UniformBuffer
	{
	public:

		virtual ~UniformBuffer() = default;

		virtual void SetData(const void* data, uint64_t size , uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint64_t size, uint32_t binding);
	};
}