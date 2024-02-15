#pragma once
#include <vector>
#include "ErrorChecker.h"
#include "VertexBuffer.h"
#include "VertexLayoutManager.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"

// this class assumes that the vertices are made of floats and that each vertexArray only has one buffer/layout pair
class Mesh
{
public:
    Mesh() {}
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout)
        //: indexBuffer(indices.data(), indices.size()), vertexBuffer(vertices.data(), vertices.size() * sizeof(float))
        { Setup(vertices, indices, layout); }
    void Setup(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout);

    void Bind() const;
    static void UnBind();
    inline unsigned int GetIndexCount() const { return indexBuffer.GetCount(); }
    uuids::uuid GetID() const { return id; }
    static Mesh CreateSquare(); // if you make more primitive meshes, then move this into a separate class

private:
    uuids::uuid id = UuidCreator::GetUnInitializedID();
    VertexBuffer vertexBuffer;
    VertexLayoutManager layoutManager;
    VertexArray vertexArray;
    IndexBuffer indexBuffer;
    // evt. store vertices and indices, but be aware that this will either make a copy 
    // or refer to external data, which might get deallocated
    

};

