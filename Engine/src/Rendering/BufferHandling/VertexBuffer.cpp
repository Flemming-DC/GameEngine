#include "VertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"


void VertexBuffer::Setup(const void* data, unsigned int size)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("VertexBuffer cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("VertexBuffer is already initialized");
    id = UuidCreator::MakeID();
    glCall(glGenBuffers(1, &openGLid));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, openGLid));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    Log(" VertexBuffer destroyed with openGLid = " + std::to_string(openGLid));
    if (UuidCreator::IsInitialized(id))
        glCall(glDeleteBuffers(1, &openGLid));
}

void VertexBuffer::Bind() const
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("You cannot bind an uninitialized VertexBuffer");
    glCall(glBindBuffer(GL_ARRAY_BUFFER, openGLid));
}

void VertexBuffer::UnBind()
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}






