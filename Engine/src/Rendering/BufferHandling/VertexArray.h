#pragma once
#include "VertexBuffer.h"
#include "VertexLayoutManager.h"

class VertexArray
{
private:
	unsigned int rendererID;
	//VertexBuffer vertexBuffer;

public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	static void UnBind();

	void AddBuffer(const VertexBuffer& vb, const VertexLayoutManager& layout) const;

	template<typename T>
	void Push(T value) {}

};

