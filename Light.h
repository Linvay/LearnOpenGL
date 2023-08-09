#pragma once

#include <glad/glad.h>
#include <iostream>

#include "Shader.h"


std::string getIndexedUniformName(std::string name, int index, std::string attribute)
{
	return (name + '[' + char('0' + index) + "]." + attribute);
}



class DirLight
{
public:
	DirLight(
		Shader& shader,
		float ambient,
		float diffuse,
		float specular,
		const glm::vec3& direction
	)
	{
		DirLight::ambient = ambient;
		DirLight::diffuse = diffuse;
		DirLight::specular = specular;
		DirLight::direction = direction;

		shader.Activate();
		shader.SetVec3("dirLight.ambient", glm::vec3(ambient));
		shader.SetVec3("dirLight.diffuse", glm::vec3(diffuse));
		shader.SetVec3("dirLight.specular", glm::vec3(specular));
		shader.SetVec3("dirLight.direction", direction);
	}
private:
	float ambient;
	float diffuse;
	float specular;
	glm::vec3 direction;
};

class PointLight
{
public:
	PointLight(
		Shader& shader,
		float ambient,
		float diffuse,
		float specular,
		const glm::vec3& position,
		float constant = 1.0f,
		float linear = 0.09f,
		float quadratic = 0.032f
	)
	{
		PointLight::ambient = ambient;
		PointLight::diffuse = diffuse;
		PointLight::specular = specular;
		PointLight::constant = constant;
		PointLight::linear = linear;
		PointLight::quadratic = quadratic;
		PointLight::position = position;

		shader.Activate();
		shader.SetVec3("pointLight.ambient", glm::vec3(ambient));
		shader.SetVec3("pointLight.diffuse", glm::vec3(diffuse));
		shader.SetVec3("pointLight.specular", glm::vec3(specular));
		shader.SetFloat("pointLight.constant", constant);
		shader.SetFloat("pointLight.linear", linear);
		shader.SetFloat("pointLight.quadratic", quadratic);
		shader.SetVec3("pointLight.position", position);
	}
private:
	float ambient;
	float diffuse;
	float specular;
	float constant;
	float linear;
	float quadratic;
	glm::vec3 position;
};

class SpotLight
{
public:
	SpotLight(
		Shader& shader,
		float ambient,
		float diffuse,
		float specular,
		const glm::vec3& position,
		const glm::vec3& direction,
		float constant = 1.0f,
		float linear = 0.09f,
		float quadratic = 0.032f,
		float cutOff = glm::cos(glm::radians(12.5f)),
		float outerCutOff = glm::cos(glm::radians(15.0f))
	)
	{
		SpotLight::ambient = ambient;
		SpotLight::diffuse = diffuse;
		SpotLight::specular = specular;
		SpotLight::constant = constant;
		SpotLight::linear = linear;
		SpotLight::quadratic = quadratic;
		SpotLight::cutOff = cutOff;
		SpotLight::outerCutOff = outerCutOff;
		SpotLight::position = position;
		SpotLight::direction = direction;

		SpotLight::index = count;
		count += 1;

		shader.Activate();
		shader.SetVec3(getIndexedUniformName("spotLights", index, "ambient"), glm::vec3(ambient));
		shader.SetVec3(getIndexedUniformName("spotLights", index, "diffuse"), glm::vec3(diffuse));
		shader.SetVec3(getIndexedUniformName("spotLights", index, "specular"), glm::vec3(specular));
		shader.SetFloat(getIndexedUniformName("spotLights", index, "constant"), constant);
		shader.SetVec3(getIndexedUniformName("spotLights", index, "specular"), glm::vec3(specular));
		shader.SetFloat(getIndexedUniformName("spotLights", index, "constant"), constant);
		shader.SetFloat(getIndexedUniformName("spotLights", index, "linear"), linear);
		shader.SetFloat(getIndexedUniformName("spotLights", index, "quadratic"), quadratic);
		shader.SetFloat(getIndexedUniformName("spotLights", index, "cutOff"), cutOff);
		shader.SetFloat(getIndexedUniformName("spotLights", index, "outerCutOff"), outerCutOff);

		shader.SetVec3(getIndexedUniformName("spotLights", index, "position"), position);
		shader.SetVec3(getIndexedUniformName("spotLights", index, "position"), position);
	}
private:
	static int count;
	int index;

	float ambient;
	float diffuse;
	float specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
	glm::vec3 position;
	glm::vec3 direction;
};