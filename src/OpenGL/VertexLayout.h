#pragma once
#include <GLAD/glad.h>
#include <vector>

struct VertexBufferElement {
    unsigned int glType;
    unsigned int count;
    unsigned char normalized;


    static unsigned int getSizeOfType(unsigned int type) {
        switch (type) {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
    unsigned int count;
public:
    VertexBufferLayout();
    VertexBufferLayout(const VertexBufferLayout& other);
    template<typename T>
    void push(unsigned int count);

    inline const std::vector<VertexBufferElement> getElements() const { return elements; }
    inline unsigned int getStride() const { return stride; }
    inline unsigned int getCount() const { return count; }

};

template<>
void VertexBufferLayout::push<float>(unsigned int count);

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count);

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count);
