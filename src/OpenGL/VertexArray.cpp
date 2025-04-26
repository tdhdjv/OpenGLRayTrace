#include <GLAD/glad.h>
#include "VertexArray.h"
#include "Core/Core.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::setAttrib(const VertexBufferLayout& layout) const {
    bind();
    const auto& elements = layout.getElements();
    unsigned short offset = 0;
    unsigned int stride = layout.getStride();
    for (unsigned int i = 0; i < elements.size(); i++) {
        VertexBufferElement element = elements[i];
        GLCall(glVertexAttribPointer(i, element.count, element.glType, element.normalized, stride, (const void*)offset));
        GLCall(glEnableVertexAttribArray(i));
        offset += element.count * element.getSizeOfType(element.glType);
    }
    unBind();
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(rendererID));
}

void VertexArray::unBind() const {
    GLCall(glBindVertexArray(0));
}