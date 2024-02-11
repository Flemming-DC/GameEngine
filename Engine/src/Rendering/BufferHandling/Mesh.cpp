#include "Mesh.h"


Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout)
    : indexBuffer(indices.data(), indices.size()),
    vertexBuffer(vertices.data(), vertices.size() * sizeof(float))
{
    layoutManager.Push<float>(layout.positionDimension);
    layoutManager.Push<float>(layout.textureDimension);
    layoutManager.Push<float>(layout.colorDimension);
    layoutManager.Push<float>(layout.textureID);
    vertexArray.AddBuffer(vertexBuffer, layoutManager);
}

void Mesh::Bind() const
{
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
