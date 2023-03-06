#pragma once

#include "Buffer.h"

struct FUniformBufferObject
{
	glm::mat4 m_Model;

	glm::mat4 m_View;

	glm::mat4 m_Projection;
};

class UniformBuffer : public Buffer
{
public:
	UniformBuffer(class Device* device, uint32_t size);

	void Update(uint32_t size, void* data);

private:

	void* m_MappedUniformBuffers = nullptr;
};