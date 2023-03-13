#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(binding = 1) uniform sampler2D m_TextureSampler;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor = texture(m_TextureSampler, fragTexCoord);
	//outColor = vec4(fragTexCoord, 0.0 ,1.0);
}