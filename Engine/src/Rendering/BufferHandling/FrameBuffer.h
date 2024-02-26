#pragma once
#include "UuidCreator.h"
#include "Register.h"

class FrameBuffer
{
public:
	static Register<FrameBuffer> register_;

	FrameBuffer() {}
	FrameBuffer(int width, int height) { Setup(width, height); }
	void ShutDown();
	void Setup(int width, int height);

	void Bind() const;
	static void UnBind();
	void Draw();
	inline uuids::uuid GetID() const { return id; }
	inline unsigned int GetTextureOpenGLid() const { return texture_openGLid; }
	// resize / invalidate recreate state

private:
	unsigned int openGLid = 0;
	unsigned int texture_openGLid = -1;
	unsigned int renderBuffer_openGLid = -1;
	int width = -1; // all framebuffer components must have the same with and height, where to get width and height from?
	int height = -1;
	//uuids::uuid textureID;
	uuids::uuid id;
};

