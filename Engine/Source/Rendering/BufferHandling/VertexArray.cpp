#include "VertexArray.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGlError.h"
#include "OpenGlSetup.h"
#include "OpenGLidChecker.h"

Register<VertexArray> VertexArray::register_;

void VertexArray::Setup()
{
    if (!OpenGlSetup::Initialized())
        RaiseError("VertexArray cannot be setup before OpenGlSetup::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("VertexArray is already initialized");
    id = UuidCreator::MakeID();
    glCall(glGenVertexArrays(1, &openGLid));
    glCall(glBindVertexArray(openGLid));
    //OpenGLidChecker::Add(Tools::to_string(*this), openGLid);
    //Log("VertexArray setup with openGLid = " + std::to_string(openGLid));

}
void VertexArray::ShutDown()
{
    if (!UuidCreator::IsInitialized(id) && openGLid != 0)
        RaiseError("Uninitialized VertexArray has openGLid != 0");
    if (!UuidCreator::IsInitialized(id))
        return;
    glCall(glDeleteVertexArrays(1, &openGLid));
    //OpenGLidChecker::Remove(Tools::to_string(*this), openGLid);

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


std::string VertexArray::to_string() const
{
    std::string newline = "\n    ";
    std::string out = "VertexArray:" + newline;

    int boundOpenGLid;
    glCall(glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundOpenGLid));

    out += "id: " + logger::to_string(id) + newline;
    out += "openGLid: " + std::to_string(openGLid) + newline;
    out += "isBound: " + std::to_string(openGLid == boundOpenGLid) + newline;

    return out;
}



