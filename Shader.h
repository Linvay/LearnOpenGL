#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
public:
	GLuint ID = 0;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

	// Uniform variable setting function
	void SetBool(const char* uniformVarName, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, uniformVarName), (int)value);
	}
	void SetInt(const char* uniformVarName, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, uniformVarName), value);
	}
	void SetFloat(const char* uniformVarName, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, uniformVarName), value);
	}

	void SetVec2(const char* uniformVarName, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, uniformVarName), 1, &value[0]);
	}
	void SetVec2(const char* uniformVarName, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, uniformVarName), x, y);
	}

	void SetVec3(const char* uniformVarName, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, uniformVarName), 1, &value[0]);
	}
	void SetVec3(const char* uniformVarName, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, uniformVarName), x, y, z);
	}

	void SetVec4(const char* uniformVarName, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, uniformVarName), 1, &value[0]);
	}
	void SetVec4(const char* uniformVarName, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, uniformVarName), x, y, z, w);
	}

	void SetMat2(const char* uniformVarName, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, uniformVarName), 1, GL_FALSE, &mat[0][0]);
	}
	void SetMat3(const char* uniformVarName, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, uniformVarName), 1, GL_FALSE, &mat[0][0]);
	}
	void SetMat4(const char* uniformVarName, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, uniformVarName), 1, GL_FALSE, &mat[0][0]);
	}

private:
	void CheckCompileErrors(GLuint shader, std::string type);
};