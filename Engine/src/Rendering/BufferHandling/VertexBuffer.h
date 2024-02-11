#pragma once

class VertexBuffer
{
private:
	unsigned int rendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	static void UnBind();

};

