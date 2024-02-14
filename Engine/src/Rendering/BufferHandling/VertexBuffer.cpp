#include "VertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"


void VertexBuffer::Setup(const void* data, unsigned int size)
{
    if (UuidCreator::IsInitialized(id))
        RaiseError("VertexBuffer is already initialized");
    id = UuidCreator::MakeID();
    glCall(glGenBuffers(1, &rendererID));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    glCall(glDeleteBuffers(1, &rendererID));
}

void VertexBuffer::Bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::UnBind()
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}






