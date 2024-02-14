#include "Mesh.h"
#include "Initializer.h"


void Mesh::Setup(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout)
{
    if (!Initializer::OpenGLInitialized())
        RaiseError("Mesh cannot be setup before Initializer::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("Mesh is already initialized");
    id = UuidCreator::MakeID();
    //indexBuffer = IndexBuffer(indices.data(), indices.size());
    //vertexBuffer = VertexBuffer(vertices.data(), vertices.size() * sizeof(float));

    layoutManager.Push<float>(layout.positionDimension);
    layoutManager.Push<float>(layout.textureDimension);
    layoutManager.Push<float>(layout.colorDimension);
    layoutManager.Push<float>(layout.textureID);
    vertexArray.Setup();
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



Mesh Mesh::CreateSquare()
{
    std::vector<float> vertexVector =
    {//  pos.x  pos.y tex.u tex.v
        -0.5f, -0.5f, 0.0f, 0.0f, // LD
        -0.5f,  0.5f, 0.0f, 1.0f, // LU
         0.5f,  0.5f, 1.0f, 1.0f, // RU
         0.5f, -0.5f, 1.0f, 0.0f, // RD
    };
    std::vector<unsigned int> indexVector =
    {
        0, 3, 2, // triangle(LD, RD, RU)
        0, 1, 2, // triangle(LD, LU, RU)
    };
    return Mesh(vertexVector, indexVector, VertexLayout());
}
