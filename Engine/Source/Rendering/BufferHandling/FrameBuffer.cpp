#include "FrameBuffer.h"
#include "Texture.h"
#include "OpenGlExternal.h"
#include "OpenGlError.h"
#include "OpenGlSetup.h"
#include "OpenGLidChecker.h"

Register<FrameBuffer> FrameBuffer::register_;

void FrameBuffer::Setup(int width_, int height_)
{
    Assert(OpenGlSetup::Initialized(),
        "FrameBuffer cannot be setup before OpenGlSetup::Setup() is called.");
    Deny(UuidCreator::IsInitialized(id),
        "FrameBuffer is already initialized");
    id = UuidCreator::MakeID();

    width = width_;
    height = height_;

    glCall(glGenFramebuffers(1, &openGLid));
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, openGLid));

    // color and depth can be stored in texture or renderbuffer. This latter is fastest, but cannot be accessed from a shader
    // 
    // make texture for color
    glCall(glGenTextures(1, &texture_openGLid));
    glCall(glBindTexture(GL_TEXTURE_2D, texture_openGLid));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // Texture GL_LINEAR vs GL_NEAREST
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // Texture GL_LINEAR vs GL_NEAREST
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_openGLid, 0));

    // make renderbuffer for depth and stencil
    glCall(glGenRenderbuffers(1, &renderBuffer_openGLid));
    glCall(glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_openGLid));
    glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer_openGLid));

    glCall(unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER)); // fboStatus = 36053
    Assert(fboStatus == GL_FRAMEBUFFER_COMPLETE,
        "framebuffer failed with ErrorCode: " + std::to_string(fboStatus));

    UnBind();
    glCall(glBindTexture(GL_TEXTURE_2D, 0)); // unbind
    glCall(glBindRenderbuffer(GL_RENDERBUFFER, 0)); // unbind
    OpenGLidChecker::Add(Tools::TypeName(*this), openGLid);
    OpenGLidChecker::Add(Tools::TypeName(*this) + ".Texture", texture_openGLid);
    OpenGLidChecker::Add(Tools::TypeName(*this) + ".RenderBuffer", renderBuffer_openGLid);
}

void FrameBuffer::ShutDown()
{
    Assert(UuidCreator::IsInitialized(id) && openGLid != 0,
        "Uninitialized VertexBuffer has openGLid != 0");
    if (!UuidCreator::IsInitialized(id))
        return;
    glCall(glDeleteFramebuffers(1, &openGLid)); 
    glCall(glDeleteTextures(1, &texture_openGLid));
    glCall(glDeleteRenderbuffers(1, &renderBuffer_openGLid));
    OpenGLidChecker::Remove(Tools::TypeName(*this), openGLid);
    OpenGLidChecker::Remove(Tools::TypeName(*this) + ".Texture", texture_openGLid);
    OpenGLidChecker::Remove(Tools::TypeName(*this) + ".RenderBuffer", renderBuffer_openGLid);
}

void FrameBuffer::Bind() const
{
    Assert(UuidCreator::IsInitialized(id),
        "You cannot bind an uninitialized FrameBuffer");
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, openGLid));
    // only bind texture and renderBuffer if you need to edit them as opposed to simply displaying them
}

void FrameBuffer::UnBind()
{
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}




