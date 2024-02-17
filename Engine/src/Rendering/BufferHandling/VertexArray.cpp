#include "VertexArray.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ErrorChecker.h"
#include "Initializer.h"

Register<VertexArray> VertexArray::register_;

void VertexArray::Setup()
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("VertexArray cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("VertexArray is already initialized");
    id = UuidCreator::MakeID();
    glCall(glGenVertexArrays(1, &openGLid));
    glCall(glBindVertexArray(openGLid));

}
void VertexArray::ShutDown()
{
    Log(" VertexArray destroyed with openGLid = " + std::to_string(openGLid));
    if (!UuidCreator::IsInitialized(id) && openGLid != 0)
        RaiseError("Uninitialized VertexArray has openGLid != 0");
    if (UuidCreator::IsInitialized(id))
        glCall(glDeleteVertexArrays(1, &openGLid));
}

void VertexArray::Bind() const
{
    glCall(glBindVertexArray(openGLid));
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





