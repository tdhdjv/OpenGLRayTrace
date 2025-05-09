#include "ShaderStorageBuffer.h"

ShaderStorageBuffer::ShaderStorageBuffer() {}

ShaderStorageBuffer::ShaderStorageBuffer(unsigned bindingLocation, list<SSBOData> dataList): bindingLocation(bindingLocation) {
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer));

	size_t totalSize = 0;
	for (SSBOData data : dataList) totalSize += data.size;
	GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, totalSize, nullptr, GL_DYNAMIC_COPY));

	size_t offset = 0;
	for (SSBOData data : dataList) {
		GLCall(glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, data.size, data.ptr));
		offset += data.size;
	}
	GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingLocation, buffer));
	GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

ShaderStorageBuffer& ShaderStorageBuffer::operator=(ShaderStorageBuffer&& other) noexcept {
	buffer = other.buffer;
	bindingLocation = other.bindingLocation;
	other.buffer = 0;
	return *this;
}

ShaderStorageBuffer::~ShaderStorageBuffer() {
	if (buffer == 0) return;
	GLCall(glDeleteBuffers(1, &buffer));
}
