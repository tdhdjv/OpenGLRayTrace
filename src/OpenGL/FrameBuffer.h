#pragma once

#include "OpenGL/RenderBuffer.h"

class FrameBuffer {
public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void bind();
	void unbind();

	void bindColorBuffer(int location);
private:
	unsigned rendererID;
	unsigned colorBufferTexture;
	RenderBuffer rbo;
};