#include "Texture.h"
#include "ErrorChecker.h"
#include "stb_image/stb_image.h"
#include "Initializer.h"


void Texture::Setup(const std::string& filePath_)
{
	if (!Initializer::OpenGLInitialized())
		RaiseError("Texture cannot be setup before Initializer::Setup() is called.");
	if (UuidCreator::IsInitialized(id))
		RaiseError("Texture is already initialized");
	id = UuidCreator::MakeID();
	filePath = filePath_;
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(filePath.c_str(), &width, &height, &bytesPerPixel, 4); // 4 = channel count
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
	
	if (localBuffer)
		stbi_image_free(localBuffer);
}

Texture::~Texture()
{
	Log(" Texture destroyed with openGLid = " + std::to_string(openGLid));
	if (!UuidCreator::IsInitialized(id) && openGLid != 0)
		RaiseError("Uninitialized Texture has openGLid != 0");
	if (UuidCreator::IsInitialized(id))
		glCall(glDeleteTextures(1, &openGLid));
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
