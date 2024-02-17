#include "IndexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"

Register<IndexBuffer> IndexBuffer::register_;

void IndexBuffer::Setup(const unsigned int* data, unsigned int count_)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("IndexBuffer cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("IndexBuffer is already initialized");
    id = UuidCreator::MakeID();
    static_assert(sizeof(unsigned int) == sizeof(GLuint),
        "IndexBuffer expects sizeof(unsigned int) == sizeof(GLuint)");
    count = count_;
    
    glCall(glGenBuffers(1, &openGLid));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLid));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::ShutDown()
{
    Log(" IndexBuffer destroyed with openGLid = " + std::to_string(openGLid));
    if (!UuidCreator::IsInitialized(id) && openGLid != 0)
        RaiseError("Uninitialized IndexBuffer has openGLid != 0");
    if (UuidCreator::IsInitialized(id))
        glCall(glDeleteBuffers(1, &openGLid));
}

void IndexBuffer::Bind() const
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("You cannot bind an uninitialized IndexBuffer");
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLid));
}

void IndexBuffer::UnBind()
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}





