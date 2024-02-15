#pragma once
#include "VertexBuffer.h"
#include "VertexLayoutManager.h"
#include "UuidCreator.h"
#include "Register.h"

class VertexArray
{
public:
	static Register<VertexArray> register_;

	//VertexArray() {}
	//VertexArray() { Setup(); } // blank input set???
	//~VertexArray();
	//void Setup();

	VertexArray() {}
	~VertexArray();
	void Setup();

	void Bind() const;
	static void UnBind();
	void AddBuffer(const VertexBuffer& vb, const VertexLayoutManager& layout) const;
	uuids::uuid GetID() const { return id; }

	template<typename T>
	void Push(T value) {}

private:
	uuids::uuid id;
	unsigned int openGLid = 0;
	//VertexBuffer vertexBuffer;
};

