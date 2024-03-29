#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 camera;
uniform mat4 model;

void main()
{
	vec4 currentPosition = model * vec4(aPos, 1.0);
	gl_Position = camera * currentPosition;
}