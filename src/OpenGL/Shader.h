#pragma once
#include <GLAD/glad.h>

#include <GLM/glm.hpp>

#include "Core/Core.h"
#include "Core/Defines.h"
#include "UniformBuffer.h"

#include <string>
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

enum UniformType {
	INT,
	FLOAT,
	VEC3,
	VEC4,
	MAT4,
};

//TODO: FIX Later can lead to dangling pointers!!!
struct UniformElement {
	const unsigned location;
	const UniformType type;
	const void* value;

	UniformElement(unsigned location, UniformType type, const void* value):
		location(location), type(type), value(value) {}
};

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(std::initializer_list<Shader> shader);
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	~ShaderProgram();

	void attachShader(Shader&& shader);
	void linkProgram();

	inline void bind() const { glUseProgram(rendererID); }
	inline void unbind() const { glUseProgram(0); }
	inline const int getID() const { return rendererID; }

	void updateGlobalUniforms();

	void pushGlobalInt(const char* name, const int* value);
	void pushGlobalFloat(const char* name, const float* value);
	void pushGlobalFloat3(const char* name, const glm::vec3* value);
	void pushGlobalFloat4(const char* name, const glm::vec4* value);
	void pushGlobalMat4(const char* name, const glm::mat4* value);

	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void setFloat3(const char* name, float f1, float f2, float f3);
	void setFloat3(const char* name, const glm::vec3& value);
	void setFloat4(const char* name, float f1, float f2, float f3, float f4);
	void setFloat4(const char* name, const glm::vec4& value);
	void setMat4(const char* name, const glm::mat4& value);
private:
	unsigned rendererID;
	//global uniforms AKA uniforms that will change per render
	list<UniformElement> globalUniforms;
	map<const char*, unsigned> uniformLocation;

	int getLocation(const char* name);
	
	void setValue(const unsigned location, UniformType type, const void* value);

	inline void setInt(const unsigned location, int value) { GLCall(glUniform1i(location, value)); }
	inline void setFloat(const unsigned location, float value) { GLCall(glUniform1f(location, value)); }
	inline void setFloat3(const unsigned location, float f1, float f2, float f3) { GLCall(glUniform3f(location, f1, f2, f3)); }
	inline void setFloat3(const unsigned location, const glm::vec3& value) { GLCall(glUniform3f(location, value.x, value.y, value.z)); }
	inline void setFloat4(const unsigned location, float f1, float f2, float f3, float f4) { GLCall(glUniform4f(location, f1, f2, f3, f4)); }
	inline void setFloat4(const unsigned location, const glm::vec4& value) { GLCall(glUniform4f(location, value.x, value.y, value.z, value.w)); }
	inline void setMat4(const unsigned location, const glm::mat4& value) { GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0])); }
};