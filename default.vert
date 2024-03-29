#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 FragPosition;
out vec2 texCoord;

uniform mat4 camera;
uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
	vec4 currentPosition = model * vec4(aPosition, 1.0);
	gl_Position = camera * currentPosition;
	FragPosition = vec3(currentPosition);
	Normal = normalMatrix * aNormal;
	texCoord = aTexCoord;
}