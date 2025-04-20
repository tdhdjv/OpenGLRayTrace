#pragma once
#include <GLAD/glad.h>

#include <string>
#include <GLM/glm.hpp>

#include "Core/Core.h"
#include "Core/Defines.h"
#include <any>

struct ParseFile {
	std::string filePath;
	std::string source;
	std::fpos_t position;
};

class Shader {
public:
	Shader(unsigned shaderType, const char* filePath);
	~Shader();

	inline const unsigned getID() const { return rendererID; };
private:
	unsigned rendererID;
	unsigned shaderType;

	void compile(const char* filePath);
	std::string parseShader(std::string filePath);
};

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	~ShaderProgram();

	void attachShader(Shader&& shader);
	void linkProgram();

	inline void bind() const { glUseProgram(rendererID); }
	inline void unbind() const { glUseProgram(0); }

	template<typename T>
	void setUniform(std::string name, const T& value);

private:
	unsigned rendererID;
	map<std::string, int> uniformLocation;
	map<std::string, std::any> uniformValue;

	int getUniformLocation(std::string name);
};

template<>
void ShaderProgram::setUniform<int>(std::string name, const int& value);
template<>
void ShaderProgram::setUniform<float>(std::string name, const float& value);
template<>
void ShaderProgram::setUniform<glm::vec3>(std::string name, const glm::vec3& value);
template<>
void ShaderProgram::setUniform<glm::mat4>(std::string name, const glm::mat4& value);