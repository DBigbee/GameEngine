#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GE
{
	struct Transform
	{
		Transform()
			:m_Position(), m_Rotation(), m_Scale(1.0f)
		{

		}

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 ToMat4()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));
			return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}
	};
}