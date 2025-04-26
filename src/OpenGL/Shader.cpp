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

int ShaderProgram::getUniformLocation(std::string name) {
	if (uniformLocation.contains(name)) return uniformLocation[name];
	GLCall(int location = glGetUniformLocation(rendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Uniform called '" << name << "' Doesn't Exist or is Unused" << std::endl;
	}
	uniformLocation[name] = location;
	return location;
}

template<>
void ShaderProgram::setUniform<int>(std::string name, const int& value) {
	int location = getUniformLocation(name);
	if (location == -1) return;
	GLCall(glUniform1i(location, value));
}

template<>
void ShaderProgram::setUniform<float>(std::string name,const float& value) {
	int location = getUniformLocation(name);
	if (location == -1) return;
	GLCall(glUniform1f(location, value));
}

template<>
void ShaderProgram::setUniform<glm::vec3>(std::string name, const glm::vec3& value) {
	int location = getUniformLocation(name);
	if (location == -1) return;
	GLCall(glUniform3f(location, value.x, value.y, value.z));
}

template<>
void ShaderProgram::setUniform<glm::mat4>(std::string name, const glm::mat4& value) {
	int location = getUniformLocation(name);
	if (location == -1) return;
	GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
}
