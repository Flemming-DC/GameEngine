#include "IndexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"


void IndexBuffer::Setup(const unsigned int* data, unsigned int count_)
{
    if (UuidCreator::IsInitialized(id))
        RaiseError("IndexBuffer is already initialized");
    id = UuidCreator::MakeID();
    static_assert(sizeof(unsigned int) == sizeof(GLuint),
        "IndexBuffer expects sizeof(unsigned int) == sizeof(GLuint)");
    count = count_;
    
    glCall(glGenBuffers(1, &rendererID));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    glCall(glDeleteBuffers(1, &rendererID));
}

void IndexBuffer::Bind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::UnBind()
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}





