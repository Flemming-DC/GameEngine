#pragma once
#include <string>
#include "UuidCreator.h"
#include "Register.h"

class Texture
{
public:
	static Register<Texture> register_;

	Texture() {}
	Texture(const std::string& filePath) { Setup(filePath); }
	void ShutDown();
	void Setup(const std::string& filePath_);

	void Bind(unsigned int slot = 0) const;
	static void UnBind();

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline uuids::uuid GetID() const { return id; }

private:
	uuids::uuid id;
	unsigned int openGLid = 0;
	std::string filePath = "";
	unsigned char* localBuffer = nullptr;
	int width = -1;
	int height = -1;
	int bytesPerPixel = -1;
};

