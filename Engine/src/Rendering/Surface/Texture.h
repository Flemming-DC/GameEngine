#pragma once
#include <string>

class Texture
{
private: 
	unsigned int rendererID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, bytesPerPixel;

public:
	Texture(const std::string& filePath_);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	static void UnBind();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

};

