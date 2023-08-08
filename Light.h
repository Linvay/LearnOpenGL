#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Shader.h"



class Light
{
public:
	Light(
		const glm::vec3& ambient,
		const glm::vec3& difuse,
		const glm::vec3& specular
		)
	{
		Light::ambient = ambient;
		Light::diffuse = diffuse;
		Light::specular = specular;
	};

	void SetShaderData();

private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class DirLight : Light
{
public:
	DirLight(
		const glm::vec3& ambient,
		const glm::vec3& difuse,
		const glm::vec3& specular,
		const glm::vec3& direction
	) : Light(ambient, difuse, specular)
	{
		DirLight::direction = direction;
	};
private:
	glm::vec3 direction;
};