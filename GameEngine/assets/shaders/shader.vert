#version 450

layout(binding = 0) uniform UniformBufferObject
{
	mat4 m_Model;
	mat4 m_View;
	mat4 m_Proj;

} ubo;

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Color;

layout(location = 0) out vec3 fragColor;


void main()
{
	gl_Position = ubo.m_Proj * ubo.m_View * ubo.m_Model * vec4(v_Position, 1.0);
	fragColor = v_Color;
}