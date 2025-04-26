#pragma once

#include <GLAD/glad.h>

class VertexBuffer {
private:
    unsigned int rendererID;
public:
    VertexBuffer(const void* vertices, size_t size);
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    ~VertexBuffer();

    void bind() const;
    void unBind() const;
};