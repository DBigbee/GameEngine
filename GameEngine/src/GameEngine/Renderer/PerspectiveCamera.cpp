#include "pch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GE
{

	PerspectiveCamera::PerspectiveCamera()
	{
		RecalulateProjectionViewMatrix();
	}

	void PerspectiveCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_Aspect = (float)width / (float)height;
	}

	void PerspectiveCamera::RecalulateProjectionViewMatrix()
	{
		m_Projection = glm::perspective(glm::radians(45.0f), m_Aspect, m_NearClip, m_FarClip);
	}
}

