#include "FrameBuffer.h"
#include "Texture.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"
#include "OpenGLidChecker.h"

Register<FrameBuffer> FrameBuffer::register_;

void FrameBuffer::Setup(int width_, int height_)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("FrameBuffer cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("FrameBuffer is already initialized");
    id = UuidCreator::MakeID();

    width = width_;
    height = height_;

    glCall(glGenFramebuffers(1, &openGLid));
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, openGLid));

    // color and depth can be stored in texture or renderbuffer. This latter is fastest, but cannot be accessed from a shader
    // make texture for color (Texture GL_LINEAR vs GL_NEAREST)
    glCall(glGenTextures(1, &texture_openGLid));
    glCall(glBindTexture(GL_TEXTURE_2D, texture_openGLid));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // Texture GL_LINEAR vs GL_NEAREST
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)); // Texture GL_LINEAR vs GL_NEAREST
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_openGLid, 0));

    // make renbuffer for depth and stencil
    glCall(glGenRenderbuffers(1, &renderBuffer_openGLid));
    glCall(glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer_openGLid));
    glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
    glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer_openGLid));

    glCall(unsigned int fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER)); // fboStatus = 36053
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        Log("framebuffer failed with errorcode: " + std::to_string(fboStatus));

    OpenGLidChecker::Add(Tools::type_as_string(*this), openGLid);
}

void FrameBuffer::ShutDown()
{
    if (!UuidCreator::IsInitialized(id) && openGLid != 0)
        RaiseError("Uninitialized VertexBuffer has openGLid != 0");
    if (!UuidCreator::IsInitialized(id))
        return;
    glCall(glDeleteFramebuffers(1, &openGLid)); // hyp
    glCall(glDeleteTextures(1, &texture_openGLid)); // hyp
    glCall(glDeleteRenderbuffers(1, &renderBuffer_openGLid)); // hyp
    OpenGLidChecker::Remove(Tools::type_as_string(*this), openGLid);
}

void FrameBuffer::Bind() const
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("You cannot bind an uninitialized FrameBuffer");
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, openGLid));
    // bind texture and renderBuffer ?
}

void FrameBuffer::UnBind()
{
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // hyp
}

void FrameBuffer::Draw()
{
    Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // draw scene (presumably this gets drawn to framebuffer)

    // draw framebuffer to rectangle which is then drawn to screen
    UnBind();
    // activate framebufferprogram (whatever that is) this is a shader program
    glDisable(GL_DEPTH_TEST);
    unsigned int texture_openGLid = -1;
    glBindTexture(GL_TEXTURE_2D, texture_openGLid);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // swap buffers and poll events
}




