#pragma once
#include <vector>
#include "GlmTools.h"
#include "ErrorChecker.h"
#include "VertexBuffer.h"
#include "VertexLayoutManager.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexLayout.h"
#include "Naming.h"

// this class assumes that the vertices are made of floats and that each vertexArray only has one buffer/layout pair
class Mesh
{
    Shorts;
public:
    static Register<Mesh> register_;
    static Naming naming;
    //struct Bounds2D { float minX; float minY; float maxX; float maxY; };

    Mesh() {}
    Mesh(string name, const vector<float>& vertices, const vector<unsigned int>& indices, const VertexLayout& layout)
        { Setup(name, vertices, indices, layout); }
    void Setup(string name, const vector<float>& vertices, const vector<unsigned int>& indices, const VertexLayout& layout);

    void Bind() const;
    static void UnBind();
    inline unsigned int GetIndexCount() const { return indexBuffer.GetCount(); }
    uuid GetID() const { return id; }
    string to_string() const;
    vector<vec2> FindPositions2D();
    //Bounds2D FindBounds2D() const; // minX, minY, maxX, maxY

private:
    uuid id;
    VertexBuffer vertexBuffer;
    VertexLayoutManager layoutManager;
    VertexArray vertexArray;
    IndexBuffer indexBuffer;
    // using a ref to buffers might reduce data size, but make Mesh uncopyable and cause them to behave non-independently.

    vector<float> vertices;
    vector<unsigned int> indices; // for debug only. You could save space by removing them rfom release build
    VertexLayout layout; // for debug only. You could save space by removing them rfom release build
    

};

