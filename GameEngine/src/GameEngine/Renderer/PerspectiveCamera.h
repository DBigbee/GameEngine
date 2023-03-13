#pragma once

#include "Camera.h"

namespace GE
{
	class PerspectiveCamera : public Camera
	{
	public:

		PerspectiveCamera();

		void SetViewportSize(uint32_t width, uint32_t height);

	private:

		void RecalulateProjectionViewMatrix();

	private:

		float m_FOV = glm::radians(45.0f);
		float m_Aspect = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;
		
	};
}