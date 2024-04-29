#pragma once
#include <string>
#include "UuidCreator.h"
#include "Register.h"
#include "Naming.h"

class Texture
{
public:
	static Register<Texture> register_;
	static Naming naming;

	Texture() {}
	Texture(const std::string& filePath) { Setup(filePath); }
	void ShutDown();
	void Setup(const std::string& filePath_);

	void Bind(unsigned int slot = 0) const;
	static void UnBind();
	inline uuids::uuid GetID() const { return id; }
	inline unsigned int GetOpenGLid() const { return openGLid; }
	std::string to_string() const;

private:
	uuids::uuid id;
	unsigned int openGLid = 0;
	std::string filePath = "";
	int width = -1;
	int height = -1;
	int bytesPerPixel = -1;
};

