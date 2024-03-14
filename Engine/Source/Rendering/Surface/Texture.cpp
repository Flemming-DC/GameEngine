#include "Texture.h"
#include "OpenGlError.h"
#include "stb_image/stb_image.h"
#include "OpenGlSetup.h"
#include "OpenGLidChecker.h"

Register<Texture> Texture::register_;

void Texture::Setup(const std::string& filePath_)
{
	if (!OpenGlSetup::Initialized())
		RaiseError("Texture cannot be setup before OpenGlSetup::Setup() is called.");
	if (UuidCreator::IsInitialized(id))
		RaiseError("Texture is already initialized");
	id = UuidCreator::MakeID();
	filePath = filePath_;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* localBuffer = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 4); // 4 = channel count
	if (!localBuffer)
		RaiseError("Failed to load texture image " + filePath + "due to " + stbi_failure_reason());
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
	if (!UuidCreator::IsInitialized(id) && openGLid != 0)
		RaiseError("Uninitialized Texture has openGLid != 0");
	if (!UuidCreator::IsInitialized(id))
		return;
	glCall(glDeleteTextures(1, &openGLid));
	OpenGLidChecker::Remove(Tools::TypeName(*this), openGLid);
}

void Texture::Bind(unsigned int slot) const
{
	if (!UuidCreator::IsInitialized(id))
		RaiseError("You cannot bind an uninitialized Texture");

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