#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

uniform mat4 camera;
uniform mat4 model;
uniform mat3 normalMatrix;
uniform float outlining;

void main()
{
	vec4 currentPosition = model * vec4(aPosition + aNormal * outlining, 1.0f);
	gl_Position = camera * currentPosition;
}
