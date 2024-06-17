#include "Mesh.h"
#include "OpenGlSetup.h"
#include "EngineAssets.h"

Register<Mesh> Mesh::register_;
Naming Mesh::naming;

void Mesh::Setup(string name, const vector<float>& vertices_, const vector<unsigned int>& indices_, const VertexLayout& layout_)
{
    Assert(OpenGlSetup::Initialized(),
        "Mesh cannot be setup before OpenGlSetup::Setup() is called.");
    Deny(UuidCreator::IsInitialized(id),
        "Mesh is already initialized");
    if (!naming.Contains(name))
        naming.AddWithSuffix(name, UuidCreator::MakeID());
    id = naming.at(name);

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

    float safetyFactor = 1.1f;
    for (const vec2 pos : FindPositions2D())
    {
        if (glm::SqrMagnitude(pos) > maxExtention * maxExtention)
            maxExtention = safetyFactor * glm::Magnitude(pos);
    }
    
}


void Mesh::Bind() const
{
    Assert(UuidCreator::IsInitialized(id),
        "You cannot bind an uninitialized Mesh");
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

    out += "id: "            + logger::to_string(id)                               + newline;
    out += "vertexBuffer: "  + Tools::Replace(vertexBuffer.to_string() , "\n", newline) + newline;
    out += "layoutManager: " + Tools::Replace(layoutManager.to_string(), "\n", newline) + newline;
    out += "vertexArray: "   + Tools::Replace(vertexArray.to_string()  , "\n", newline) + newline;
    out += "indexBuffer: " + Tools::Replace(indexBuffer.to_string(), "\n", newline) + newline;
    

    out += "vertices: " + logger::to_string(vertices) + newline;
    out += "indices: " + logger::to_string(indices) + newline;
    out += "layout: " + layout.to_string() + newline;

    return out;
}

std::vector<glm::vec2> Mesh::FindPositions2D()
{
    Assert(layout.positionDimension == 2,
        "calling GetPositions2D, but layout.positionDimension isn't 2.");
    int layoutIndex = 0;
    int layoutSize = layout.positionDimension + layout.textureDimension + layout.colorDimension + layout.textureID;
    std::vector<glm::vec2> positions;

    for (const float datum : vertices)
    {
        // append position data
        if (layoutIndex == 0)
            positions.push_back(glm::vec2(datum, 0.0f));
        else if (layoutIndex == 1)
        {
            auto pos = positions.back();
            pos.y = datum;
            positions.back() = pos;
        }

        // ignore other data

        // increment layoutIndex in cycles from 0 to layoutSize - 1
        layoutIndex++;
        if (layoutIndex >= layoutSize)
            layoutIndex = 0;
    }
    return positions;
}
/*
Mesh::Bounds2D Mesh::FindBounds2D() const
{
    if (layout.positionDimension != 2)
        RaiseError("calling GetPositions2D, but layout.positionDimension isn't 2.");
    int layoutIndex = 0;
    int layoutSize = layout.positionDimension + layout.textureDimension + layout.colorDimension + layout.textureID;
    float minX = INFINITY;
    float minY = INFINITY;
    float maxX = -INFINITY;
    float maxY = -INFINITY;


    for (const float datum : vertices)
    {
        // append position data
        if (layoutIndex == 0)
        {
            if (datum < minX)
                minX = datum;
            if (datum > maxX)
                maxX = datum;
        }

        else if (layoutIndex == 1)
        {
            if (datum < minY)
                minY = datum;
            if (datum > maxY)
                maxY = datum;
        }

        // ignore other data

        // increment layoutIndex in cycles from 0 to layoutSize - 1
        layoutIndex++;
        if (layoutIndex >= layoutSize)
            layoutIndex = 0;
    }
    if (minX == INFINITY || minY == INFINITY || maxX == -INFINITY || maxY == -INFINITY)
        RaiseError("failed to find bounds2D");

    return { minX, minY, maxX, maxY };

}
*/