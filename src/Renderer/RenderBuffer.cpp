#include "RenderBuffer.h"

#include <GLAD/glad.h>

#include "Core/Core.h"

RenderBuffer::RenderBuffer(int width, int height) {
	GLCall(glGenRenderbuffers(1, &rendererID));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rendererID));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
}

RenderBuffer::~RenderBuffer() {
	GLCall(glDeleteRenderbuffers(1, &rendererID));
}

void RenderBuffer::bind() const {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rendererID));
}

void RenderBuffer::unbind() const {
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}
