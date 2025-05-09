#include "Shader.h"

#include <iostream>
#include <fstream>
#include <stack>

static const char* shaderTypeToString(unsigned shaderType) {
	switch (shaderType) {
	case GL_VERTEX_SHADER:
		return "Vertex";
	case GL_FRAGMENT_SHADER:
		return "Fragment";
	case GL_GEOMETRY_SHADER:
		return "Geometry";
	case GL_COMPUTE_SHADER :
		return "Compute";
	default:
		return "Unknown";
	}
}

Shader::Shader(unsigned shaderType, const char* filePath):shaderType(shaderType) {
	rendererID = glCreateShader(shaderType);
	compile(filePath);
}

Shader::~Shader() {
	glDeleteShader(shaderType);
}

void Shader::compile(const char* filePath) {
	std::string source = parseShader(filePath);
	const char* src = source.c_str();

	glShaderSource(rendererID, 1, &src, NULL);
	glCompileShader(rendererID);

	int result;

	glGetShaderiv(rendererID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(rendererID, GL_INFO_LOG_LENGTH, &length);
		char message[1024];
		glGetShaderInfoLog(rendererID, 1024, &length, message);
		std::cout << "Failed to Compile " << shaderTypeToString(shaderType) << " Shader" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(rendererID);
	}
}

std::string Shader::parseShader(std::string filePath) {
	std::ifstream stream;

	ParseFile currentParseFile;
	list<std::string> includedFilePaths;
	std::stack<ParseFile> parseFiles;
	std::string previousSource;
	std::string source;
	std::string line;
	parseFiles.push({ filePath, source, 0});
	includedFilePaths.push_back(filePath);

	while (!parseFiles.empty()) {
		currentParseFile = parseFiles.top();
		stream.open(currentParseFile.filePath);
		stream.seekg(currentParseFile.position);
		source = currentParseFile.source + previousSource;
		previousSource = "";
		parseFiles.pop();

		if (!stream.is_open()) {
			std::cout << "Filepath: " << currentParseFile.filePath << " for " << shaderTypeToString(shaderType) << " shader Doesn't exist!" << std::endl;
			return "";
		}
		bool finished = true;
		while (std::getline(stream, line)) {
			//search for includes
			std::size_t pos = line.find("#include", 0);
			if (pos != std::string::npos) {
				std::size_t start = line.find('"', pos);
				if(start == std::string::npos) std::cout << "Failed to include filePath" << std::endl;
				std::size_t end = line.find('"', start + 1);
				std::string filePath = line.substr(start+1, end-start-1);

				//if the file is not included then add! else do nothing
				if (std::find(includedFilePaths.begin(), includedFilePaths.end(), filePath) == includedFilePaths.end()) {
					//save the state of the current file
					includedFilePaths.push_back(filePath);
					parseFiles.push({ currentParseFile.filePath, source, stream.tellg()});
					parseFiles.push({ filePath, "", 0 });
					stream.close();
					stream.clear();
					finished = false;
					break;
				}
			}
			else {
				source += line + '\n';
			}
		}
		if(finished) previousSource = source;
		stream.close();
	}
	if (includedFilePaths.size() > 1) {
		std::ofstream output(filePath.erase(filePath.find('.')) + "[Preprocessed].glsl");
		output << source;
		output.close();
	}
	return source;
}

ShaderProgram::ShaderProgram() {
	GLCall(rendererID = glCreateProgram());
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaders) {
	GLCall(rendererID = glCreateProgram());
	for (Shader shader : shaders) {
		attachShader(std::move(shader));
	}
	linkProgram();
}

ShaderProgram::~ShaderProgram() {
	GLCall(glDeleteProgram(rendererID));
}

void ShaderProgram::attachShader(Shader&& shader) {
	GLCall(glAttachShader(rendererID, shader.getID()));
}

void ShaderProgram::linkProgram() {
	GLCall(glLinkProgram(rendererID));

	int result;

	GLCall(glGetProgramiv(rendererID, GL_LINK_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetProgramiv(rendererID, GL_INFO_LOG_LENGTH, &length));
		char message[1024];
		GLCall(glGetProgramInfoLog(rendererID, 1024, &length, message));
		std::cout << "Failed to link Shader Program!" << std::endl;
		std::cout << message << std::endl;
	}
}

void ShaderProgram::updateGlobalUniforms() {
	for (const UniformElement& element : globalUniforms) {
		setValue(element.location, element.type, element.value);
	}
}

void ShaderProgram::pushGlobalInt(const char* name, const int* value) {
	int location = getLocation(name);
	globalUniforms.emplace_back(location, UniformType::INT, value);
}

void ShaderProgram::pushGlobalFloat(const char* name, const float* value) {
	int location = getLocation(name);
	globalUniforms.emplace_back(location, UniformType::FLOAT, value);
}

void ShaderProgram::pushGlobalFloat3(const char* name, const glm::vec3* value) {
	int location = getLocation(name);
	globalUniforms.emplace_back(location, UniformType::VEC3, value);
}

void ShaderProgram::pushGlobalFloat4(const char* name, const glm::vec4* value) {
	int location = getLocation(name);
	globalUniforms.emplace_back(location, UniformType::VEC4, value);
}

void ShaderProgram::pushGlobalMat4(const char* name, const glm::mat4* value) {
	int location = getLocation(name);
	globalUniforms.emplace_back(location, UniformType::MAT4, value);
}

void ShaderProgram::setInt(const char* name, int value) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	}
	glUniform1i(location, value);
}

void ShaderProgram::setFloat(const char* name, float value) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	}
	glUniform1f(location, value);
}

void ShaderProgram::setFloat3(const char* name, float f1, float f2, float f3) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	}
	GLCall(glUniform3f(location, f1, f2, f3));
}

void ShaderProgram::setFloat3(const char* name, const glm::vec3& value) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	}
	GLCall(glUniform3f(location, value.x, value.y, value.z));
}

void ShaderProgram::setFloat4(const char* name, float f1, float f2, float f3, float f4) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	}
	GLCall(glUniform4f(location, f1, f2, f3, f4));
}

void ShaderProgram::setFloat4(const char* name, const glm::vec4& value) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	}
	GLCall(glUniform4f(location, value.x, value.y, value.z, value.z));
}

void ShaderProgram::setMat4(const char* name, const glm::mat4& value) {
	int location = getLocation(name);
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' is Unused or Doesn't Exist" << std::endl;
	} 
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
}

int ShaderProgram::getLocation(const char* name) {
	int location;
	if (uniformLocation.find(name) != uniformLocation.end()) {
		location = uniformLocation[name];
	}
	else {
		GLCall(location = glGetUniformLocation(rendererID, name));
		uniformLocation[name] = location;
	}
	return location;
}

void ShaderProgram::setValue(const unsigned location, UniformType type, const void* value) {
	switch (type) {
	case UniformType::INT:
		setInt(location, *(int*)value);
	case UniformType::FLOAT:
		setFloat(location, *(float*)value);
	case UniformType::VEC3:
		setFloat3(location, *(glm::vec3*)value);
	case UniformType::VEC4:
		setFloat4(location, *(glm::vec4*)value);
	case UniformType::MAT4:
		setMat4(location, *(glm::mat4*)value);
	}
}
