#pragma once

#include "Core/Core.h"
#include "Core/Defines.h"

struct SSBOData {
	void* ptr;
	size_t size;
};

class ShaderStorageBuffer {
public:
	ShaderStorageBuffer(unsigned bindingLocation, list<SSBOData> dataList);
	~ShaderStorageBuffer();

	inline void bind() const { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer));  };
	inline void unbind() const { GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer)); };
private:
	unsigned bindingLocation;
	unsigned buffer;
};