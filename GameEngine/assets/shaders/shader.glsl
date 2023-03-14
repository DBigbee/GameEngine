#type vertex
#version 450

layout(binding = 0) uniform UniformBufferObject
{
	mat4 m_Model;
	mat4 m_View;
	mat4 m_Proj;

} ubo;

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Color;
layout(location = 2) in vec2 v_TexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

layout(push_constant) uniform Push
{
	mat4 m_Model;
} push;

void main()
{
	gl_Position = ubo.m_Proj * ubo.m_View * push.m_Model * vec4(v_Position, 1.0);
	fragColor = v_Color;
	fragTexCoord = v_TexCoord;
}

#type fragment
#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(binding = 1) uniform sampler2D m_TextureSampler;

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform Push
{
	mat4 m_Model;
} push;

void main()
{
	outColor = texture(m_TextureSampler, fragTexCoord);
	//outColor = vec4(fragTexCoord, 0.0 ,1.0);
}