#pragma once

#include <GLAD/glad.h>

class IndexBuffer {
private:
    unsigned int rendererID;
public:
    IndexBuffer(const unsigned* indices, size_t size);
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&& indexBuffer) noexcept;
    ~IndexBuffer();

    void bind() const;
    void unBind() const;
};