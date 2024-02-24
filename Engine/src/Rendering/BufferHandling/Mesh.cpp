#include "Mesh.h"
#include "Initializer.h"
#include "EngineAssets.h"

Register<Mesh> Mesh::register_;

void Mesh::Setup(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("Mesh cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("Mesh is already initialized");
    id = UuidCreator::MakeID();
    // the copying causes the old and the new vertexBuffer to share the same openGLid, which makes the 
    // destructor of the old vertexBuffer will undo the setup of the new. Thereby causing bind to fail
    // vertexBuffer = VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    
    //indexBuffer.Setup(indices.data(), indices.size());
    //vertexBuffer.Setup(vertices.data(), vertices.size() * sizeof(float));

    indexBuffer = IndexBuffer::register_.Add(indices.data(), indices.size());
    vertexBuffer = VertexBuffer::register_.Add(vertices.data(), vertices.size() * sizeof(float));
    vertexArray = VertexArray::register_.Add();
    vertexArray.Setup();

    layoutManager.Push<float>(layout.positionDimension);
    layoutManager.Push<float>(layout.textureDimension);
    layoutManager.Push<float>(layout.colorDimension);
    layoutManager.Push<float>(layout.textureID);
    vertexArray.AddBuffer(vertexBuffer, layoutManager);
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

