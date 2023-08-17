#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

#include "Shader.h"

enum textureType
{
	DIFFUSE,
	SPECULAR
};

class Texture
{
public:
	GLuint ID;
	textureType type;
	std::string path;
	Texture(const char* imagePath, textureType type, GLuint slot);

	void SetTextureUnit(Shader& shader, const char* uniformVariableName, GLuint unit);
	void Bind(GLuint slot);
	void Unbind();
	void Delete();
};