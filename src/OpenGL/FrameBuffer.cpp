#include "FrameBuffer.h"
#include "Core/Core.h"

#include <GLAD/glad.h>
#include <iostream>

FrameBuffer::FrameBuffer(int width, int height):rbo(width, height) {
	GLCall(glGenFramebuffers(1, &rendererID));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, rendererID));

	//color buffer texture
	GLCall(glGenTextures(1, &colorBufferTexture));
	GLCall(glBindTexture(GL_TEXTURE_2D, colorBufferTexture));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTexture, 0));

	//renderBuffer
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo.getID()));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Failed to Create FrameBuffer" << std::endl;
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::~FrameBuffer() {
	GLCall(glDeleteFramebuffers(1, &rendererID));
}

void FrameBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, rendererID));
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::bindColorBuffer(int location) {
	GLCall(glActiveTexture(GL_TEXTURE0 + location));
	GLCall(glBindTexture(GL_TEXTURE_2D, colorBufferTexture));
}
