#pragma once
#include <vector>
#include "GlmTools.h"
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
    static Register<Mesh> register_;
    //struct Bounds2D { float minX; float minY; float maxX; float maxY; };

    Mesh() {}
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout)
        { Setup(vertices, indices, layout); }
    void Setup(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const VertexLayout& layout);

    void Bind() const;
    static void UnBind();
    inline unsigned int GetIndexCount() const { return indexBuffer.GetCount(); }
    uuids::uuid GetID() const { return id; }
    std::string to_string() const;
    std::vector<glm::vec2> FindPositions2D();
    //Bounds2D FindBounds2D() const; // minX, minY, maxX, maxY

private:
    uuids::uuid id;
    VertexBuffer vertexBuffer;
    VertexLayoutManager layoutManager;
    VertexArray vertexArray;
    IndexBuffer indexBuffer;
    // using a ref to buffers might reduce data size, but make Mesh uncopyable and cause them to behave non-independently.

    std::vector<float> vertices;
    std::vector<unsigned int> indices; // for debug only. You could save space by removing them rfom release build
    VertexLayout layout; // for debug only. You could save space by removing them rfom release build
    

};

