#include "VertexLayout.h"

VertexBufferLayout::VertexBufferLayout()
    : stride(0), count(0) {
}
VertexBufferLayout::VertexBufferLayout(const VertexBufferLayout& other)
{
    stride = other.stride;
    count = other.count;
    elements = other.elements;
}
;

template<>
void VertexBufferLayout::push<float>(unsigned int count) {
    elements.push_back({ GL_FLOAT, count, GL_FALSE });
    this->count += count;
    stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    this->count += count;
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    this->count += count;
    stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}