#include "IndexBuffer.h"
#include "OpenGlExternal.h"
#include "OpenGlError.h"
#include "OpenGlSetup.h"
#include "OpenGLidChecker.h"


Register<IndexBuffer> IndexBuffer::register_;

void IndexBuffer::Setup(const unsigned int* data, unsigned int count_)
{
    Assert(OpenGlSetup::Initialized(),
        "IndexBuffer cannot be setup before OpenGlSetup::Setup() is called.");
    Deny(UuidCreator::IsInitialized(id),
        "IndexBuffer is already initialized");
    id = UuidCreator::MakeID();
    static_assert(sizeof(unsigned int) == sizeof(GLuint),
        "IndexBuffer expects sizeof(unsigned int) == sizeof(GLuint)");
    count = count_;
    
    glCall(glGenBuffers(1, &openGLid));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLid));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), data, GL_STATIC_DRAW));
    OpenGLidChecker::Add(Tools::TypeName(*this), openGLid);
}

void IndexBuffer::ShutDown()
{
    Assert(UuidCreator::IsInitialized(id) || openGLid == 0,
        "Uninitialized IndexBuffer has openGLid != 0");
    if (!UuidCreator::IsInitialized(id))
        return;
    glCall(glDeleteBuffers(1, &openGLid));
    OpenGLidChecker::Remove(Tools::TypeName(*this), openGLid);
}

void IndexBuffer::Bind() const
{
    Assert(UuidCreator::IsInitialized(id),
        "You cannot bind an uninitialized IndexBuffer");
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, openGLid));
}

void IndexBuffer::UnBind()
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


std::string IndexBuffer::to_string() const
{
    std::string newline = "\n    ";
    std::string out = "IndexBuffer:" + newline;

    int boundOpenGLid;
    glCall(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundOpenGLid));

    out += "id: " + logger::to_string(id) + newline;
    out += "openGLid: " + std::to_string(openGLid) + newline;
    out += "isBound: " + std::to_string(openGLid == boundOpenGLid) + newline;

    return out;
}



