#version 450


layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Color;

layout(location = 0) out vec3 fragColor;


void main()
{
	gl_Position = vec4(v_Position, 1.0);
	fragColor = v_Color;
}