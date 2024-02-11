#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count_);
	~IndexBuffer();

	void Bind() const;
	static void UnBind();

	inline unsigned int GetCount() const { return count; }
};

