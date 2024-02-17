#pragma once
#include "UuidCreator.h"
#include "Register.h"

class IndexBuffer
{

public:
	static Register<IndexBuffer> register_;

	IndexBuffer() {}
	IndexBuffer(const unsigned int* data, unsigned int count_) { Setup(data, count_); }
	void ShutDown();
	void Setup(const unsigned int* data, unsigned int count_);

	void Bind() const;
	static void UnBind();
	inline unsigned int GetCount() const { return count; }
	uuids::uuid GetID() const { return id; }

private:
	uuids::uuid id;
	unsigned int openGLid = 0;
	unsigned int count = 0;
};

