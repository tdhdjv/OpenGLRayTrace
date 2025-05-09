#include "IndexBuffer.h"
#include "Core/Core.h"
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned* indices, size_t size) {
    GLCall(glGenBuffers(1, &rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept {
    rendererID = indexBuffer.rendererID;
    indexBuffer.rendererID = 0;
}

IndexBuffer::~IndexBuffer() {
    if (!rendererID) return;
    GLCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::bind() const {
    if (!rendererID) {
        std::cout << "This IndexBuffer does not have a valid rendererID!" << std::endl;
        return;
    }
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unBind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}