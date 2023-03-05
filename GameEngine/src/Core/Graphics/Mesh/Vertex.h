#pragma once

#include <array>
#include<glm/glm.hpp>
#include <vulkan/vulkan.h>

struct Vertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Color;

	static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDscription()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescription{};
		attributeDescription[0].binding = 0;
		attributeDescription[0].location = 0;
		attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[0].offset = offsetof(Vertex, m_Position);

		
		attributeDescription[1].binding = 0;
		attributeDescription[1].location = 1;
		attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[1].offset = offsetof(Vertex, m_Color);

		return attributeDescription;
	}
};