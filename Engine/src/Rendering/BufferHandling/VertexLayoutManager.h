#pragma once
#include <vector>
#include <GL/glew.h>


struct VertexElement // is this an element type, with count being the number of instances of the type?
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	unsigned int sizeOfType;

	VertexElement(unsigned int type_, unsigned int count_, unsigned char normalized_, unsigned int sizeOfType_)
		: type(type_), count(count_), normalized(normalized_), sizeOfType(sizeOfType_)
	{
	}

};

class VertexLayoutManager
{
private:
	std::vector<VertexElement> elements;
	unsigned int stride;

public:
	VertexLayoutManager() : stride(0) {}

	inline unsigned int GetStride() const { return stride; }
	inline std::vector<VertexElement> GetElements() const { return elements; }


	template<typename T>
	void Push(unsigned int count) { static_assert(false, "missing implementation"); }

	template<>
	void Push<float>(unsigned int count)
	{
		InternalPush({ GL_FLOAT, count, GL_FALSE, sizeof(GL_FLOAT) });
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		InternalPush({ GL_UNSIGNED_INT, count, GL_FALSE, sizeof(GL_UNSIGNED_INT) });
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		InternalPush({ GL_UNSIGNED_BYTE, count, GL_TRUE, sizeof(GL_UNSIGNED_BYTE) });
	}


private:
	void InternalPush(VertexElement element)
	{
		if (element.count == 0)
			return;
		elements.push_back(element);
		stride += element.sizeOfType * element.count;
	}

};



