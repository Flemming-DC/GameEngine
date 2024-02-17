#include "VertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"
#include "OpenGLidChecker.h"

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
    OpenGLidChecker::Add(Tools::to_string(*this), openGLid);
}

void VertexBuffer::ShutDown()
{
    if (!UuidCreator::IsInitialized(id) && openGLid != 0)
        RaiseError("Uninitialized VertexBuffer has openGLid != 0");
    if (!UuidCreator::IsInitialized(id))
        return;
    glCall(glDeleteBuffers(1, &openGLid));
    OpenGLidChecker::Remove(Tools::to_string(*this), openGLid);
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






