#include "Mesh.h"
#include "Initializer.h"
#include "EngineAssets.h"

Register<Mesh> Mesh::register_;

void Mesh::Setup(const std::vector<float>& vertices_, const std::vector<unsigned int>& indices_, const VertexLayout& layout_)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("Mesh cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("Mesh is already initialized");
    id = UuidCreator::MakeID();
    
    indexBuffer = IndexBuffer::register_.Add(indices_.data(), indices_.size());
    vertexBuffer = VertexBuffer::register_.Add(vertices_.data(), vertices_.size() * sizeof(float));
    vertexArray = VertexArray::register_.Add();
    vertexArray.Setup();

    layoutManager.Push<float>(layout_.positionDimension);
    layoutManager.Push<float>(layout_.textureDimension);
    layoutManager.Push<float>(layout_.colorDimension);
    layoutManager.Push<float>(layout_.textureID);
    vertexArray.AddBuffer(vertexBuffer, layoutManager);

    vertices = vertices_;
    indices = indices_;
    layout = layout_;
}


void Mesh::Bind() const
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("You cannot bind an uninitialized Mesh");
    vertexArray.Bind();
    indexBuffer.Bind();
}

void Mesh::UnBind()
{
    VertexArray::UnBind();
    IndexBuffer::UnBind();
}

std::string Mesh::to_string() const
{
    std::string newline = "\n    ";
    std::string out = "Mesh:" + newline;

    int boundOpenGLid;
    glCall(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundOpenGLid));

    out += "id: "            + UuidCreator::to_string(id)                               + newline;
    out += "vertexBuffer: "  + Tools::Replace(vertexBuffer.to_string() , "\n", newline) + newline;
    out += "layoutManager: " + Tools::Replace(layoutManager.to_string(), "\n", newline) + newline;
    out += "vertexArray: "   + Tools::Replace(vertexArray.to_string()  , "\n", newline) + newline;
    out += "indexBuffer: " + Tools::Replace(indexBuffer.to_string(), "\n", newline) + newline;
    

    out += "vertices: " + logger::to_string(vertices) + newline;
    out += "indices: " + logger::to_string(indices) + newline;
    out += "layout: " + layout.to_string() + newline;

    return out;
}
