#pragma once

#include <glm/glm.hpp>

namespace GE
{
	class Camera
	{
	public:

		Camera() = default;
		Camera(const glm::mat4& projection) :m_Projection(projection){}

		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }

	protected:

		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);
	};
}