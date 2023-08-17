#include "Texture.h"

Texture::Texture(const char* imagePath, textureType type, GLuint slot)
{
	Texture::type = type;
	path = imagePath;

	int widthImage, heightImage, numColorChannel;
	stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	unsigned char* imageData = stbi_load(imagePath, &widthImage, &heightImage, &numColorChannel, 0);
	if (imageData)
	{
		GLenum format = GL_RGBA;
		if (numColorChannel == 1)
			format = GL_RED;
		else if (numColorChannel == 3)
			format = GL_RGB;
		else if (numColorChannel == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, widthImage, heightImage, 0, format, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture at path " << imagePath << std::endl;
	}

	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetTextureUnit(Shader& shader, const char* uniformVariableName, GLuint unit)
{
	GLuint textureUniformID = glGetUniformLocation(shader.ID, uniformVariableName);
	shader.Activate();
	glUniform1i(textureUniformID, unit);
}

void Texture::Bind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}