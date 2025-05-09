#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"

class VertexArray {
private:
    unsigned int rendererID;
public:
    VertexArray();
    VertexArray(VertexArray& other) noexcept;
    VertexArray& operator=(const VertexArray&) = delete;

    ~VertexArray();
    void setAttrib(const VertexBufferLayout& layout) const;
    void bind() const;
    void unBind() const;
};