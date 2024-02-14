#pragma once
#include "UuidCreator.h"
#include "Register.h"

class VertexBuffer
{

public:
	static Register<VertexBuffer> register_;

	VertexBuffer() {}
	VertexBuffer(const void* data, unsigned int size) { Setup(data, size); }
	~VertexBuffer();
	void Setup(const void* data, unsigned int size);

	void Bind() const;
	static void UnBind();
	uuids::uuid GetID() const { return id; }

private:
	unsigned int openGLid = 0;
	uuids::uuid id = UuidCreator::GetUnInitializedID();
};

