#include "Texture.h"
#include "OpenGlError.h"
#include "stb_image/stb_image.h"
#include "OpenGlSetup.h"
#include "OpenGLidChecker.h"
#include "EngineLiterals.h"
#include <filesystem>

Register<Texture> Texture::register_;
Naming Texture::naming;

void Texture::Setup(const std::string& filePath_)
{
	Assert(OpenGlSetup::Initialized(),
		"Texture cannot be setup before OpenGlSetup::Setup() is called.");
	Deny(UuidCreator::IsInitialized(id),
		"Texture is already initialized");
	Assert(std::filesystem::exists(filePath_),
		"Cannot load texture at '" + filePath_ + "', since there is no file there.");

	auto name = Tools::RemovePrefix(filePath_, Literals::Textures); // we assume that all textures are in the texture folder
	if (!naming.Contains(name))
	{
		logger::print("Making new textureID");
		naming.AddWithSuffix(name, UuidCreator::MakeID());
	}
	id = naming.at(name);
	filePath = filePath_;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 4); // 4 = channel count
	Assert(localBuffer,
		"Failed to load texture image " + filePath + " due to " + stbi_failure_reason());
	glCall(glGenTextures(1, &openGLid));
	glCall(glBindTexture(GL_TEXTURE_2D, openGLid));
	
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
	
	stbi_image_free(localBuffer);
	OpenGLidChecker::Add(Tools::TypeName(*this), openGLid);
}

void Texture::ShutDown()
{
	Assert(UuidCreator::IsInitialized(id) || openGLid == 0,
		"Uninitialized Texture has openGLid != 0");
	if (!UuidCreator::IsInitialized(id))
		return;
	glCall(glDeleteTextures(1, &openGLid));
	OpenGLidChecker::Remove(Tools::TypeName(*this), openGLid);
}

void Texture::Bind(unsigned int slot) const
{
	Assert(UuidCreator::IsInitialized(id),
		"You cannot bind an uninitialized Texture");

	// GL_TEXTURE0 is an int, so GL_TEXTURE0 + slot gives another GL_TEXTURE enum value. 
	glCall(glActiveTexture(GL_TEXTURE0 + slot));
	glCall(glBindTexture(GL_TEXTURE_2D, openGLid));
}

void Texture::UnBind()
{
	glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

std::string Texture::to_string() const
{
	std::string newline = "\n    ";
	std::string out = "Texture:" + newline;

	int boundOpenGLid;
	glCall(glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundOpenGLid));

	out += "id: "			 + logger::to_string(id)		 + newline;
	out += "openGLid: "		 + std::to_string(openGLid)		 + newline;
	out += "filePath: "		 + filePath						 + newline;
	out += "width: "		 + std::to_string(width)		 + newline;
	out += "height: "		 + std::to_string(height)		 + newline;
	out += "bytesPerPixel: " + std::to_string(bytesPerPixel) + newline;
	out += "isBound: " + std::to_string(openGLid == boundOpenGLid) + newline;

	return out; 
}