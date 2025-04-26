#pragma once

class RenderBuffer {
public:
	RenderBuffer(int width, int height);
	~RenderBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned getID() const { return rendererID; }
private:
	unsigned rendererID;
};