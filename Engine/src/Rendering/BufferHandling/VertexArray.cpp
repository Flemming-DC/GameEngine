#include "VertexArray.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"

void VertexArray::Setup()
{
    if (!Initializer::IsInitialized())
        RaiseError("VertexArray cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("VertexArray is already initialized");
    id = UuidCreator::MakeID();
    glCall(glGenVertexArrays(1, &rendererID));
    glCall(glBindVertexArray(rendererID));

}

VertexArray::~VertexArray()
{
    glCall(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::Bind() const
{
    glCall(glBindVertexArray(rendererID));
}

void VertexArray::UnBind()
{
    VertexBuffer::UnBind();
    glCall(glBindVertexArray(0));
}


void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexLayoutManager& layoutManager) const
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("You cannot bind an uninitialized VertexArray");

    Bind();
    vb.Bind();
    unsigned int offset = 0;
    const auto& elements = layoutManager.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glCall(glEnableVertexAttribArray(i));
        glCall(glVertexAttribPointer(
            i, 
            element.count, 
            element.type, 
            element.normalized, 
            layoutManager.GetStride(),
            (const void*)offset)
            );
        offset += element.count * element.sizeOfType;
    }

}





