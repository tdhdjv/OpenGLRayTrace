#include "VertexBuffer.h"
#include "Core/Core.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* vertices, size_t size) {
    GLCall(glGenBuffers(1, &rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept {
    rendererID = vertexBuffer.rendererID;
    vertexBuffer.rendererID = 0;
}

VertexBuffer::~VertexBuffer() {
    if (!rendererID) return;
    GLCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::bind() const {
    if (!rendererID) {
        std::cout << "This VertexBuffer does not have a valid rendererID!" << std::endl;
        return;
    }
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unBind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}