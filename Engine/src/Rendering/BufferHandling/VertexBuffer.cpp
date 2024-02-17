#include "VertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"

Register<VertexBuffer> VertexBuffer::register_;

void VertexBuffer::Setup(const void* data, unsigned int size)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("VertexBuffer cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("VertexBuffer is already initialized");
    id = UuidCreator::MakeID();
    // The use of pointers here can cause unexpected bugs, when copying
    // eg.g. vertexBuffer = VertexBuffer(...). use Setup() instead. 
    glCall(glGenBuffers(1, &openGLid));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, openGLid));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::ShutDown()
{
    Log("ShutDown VertexBuffer with openGLid = " + std::to_string(openGLid));
    if (!UuidCreator::IsInitialized(id) && openGLid != 0)
        RaiseError("Uninitialized VertexBuffer has openGLid != 0");
    if (!UuidCreator::IsInitialized(id))
        return;
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






