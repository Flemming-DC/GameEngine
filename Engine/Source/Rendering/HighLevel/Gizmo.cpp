#include "Gizmo.h"
#include "OpenGlError.h"
#include "ListTools.h"
#include "UuidCreator.h"
#include "EngineAssets.h"

Register<Gizmo> Gizmo::register_;



Gizmo::Gizmo(std::vector<glm::vec2> position2Ds, Transform* transform_, glm::vec4 color_, bool showPoints_, bool loop_)
{
    if (UuidCreator::IsInitialized(id))
        RaiseError("Gizmo is already initialized");
    id = UuidCreator::MakeID();

    color = color_;
    showPoints = showPoints_;
    loop = loop_;
    if (transform_)
        transformID = transform_->GetID();

    std::vector<float> positionsRaw;
    for (glm::vec2 position2D : position2Ds)
    {
        positionsRaw.push_back(position2D.x);
        positionsRaw.push_back(position2D.y);
    }

    float point_size = 5;
    glCall(glPointSize(point_size));

    material = EngineAssets::GizmoMaterial(); // color is specified in draw call
    mesh = Mesh::register_.Add(positionsRaw, std::vector<unsigned int>(), VertexLayout({ 2, 0, 0, 0 }));
    positionCount = position2Ds.size();

}



void Gizmo::Draw(const glm::mat4& projectionView)
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("Cannot draw uninitialized gizmo");
    //glm::mat4 projection = Camera::Current().Projection(); // evt. save the projectionView. at least within each frame
    //glm::mat4 view = Camera::Current().View();
    glm::mat4 model = transformID ? Entity::GetComponent<Transform>(*transformID).GetModel() : glm::mat4(1.0f);
    material.SetUniform("u_MVP", projectionView * model);
    material.SetUniform("u_color", color);

    material.Bind();
    mesh.Bind();

    glCall(glDrawArrays(loop ? GL_LINE_LOOP : GL_LINE_STRIP, 0, positionCount));
    if (showPoints)
    {
        glCall(glDrawArrays(GL_POINTS, 0, positionCount));
    }
}

void Gizmo::UnBind()
{
    Mesh::UnBind();
    Material::UnBind();
}

void Gizmo::CleanupDeadGizmos()
{
    std::vector<uuids::uuid> tobeRemoved;
    for (Gizmo& gizmo : Gizmo::register_.GetData())
    {
        if (gizmo.transformID && !Entity::TryGetComponent<Transform>(*gizmo.transformID))
            tobeRemoved.push_back(gizmo.GetID());
    }
    for (uuids::uuid& gizmoID : tobeRemoved)
        Gizmo::register_.Remove(gizmoID);

}

uuids::uuid Gizmo::MakeCircle(glm::vec2 center, float radius, Transform& transform_, glm::vec4 color)
{
    std::vector<glm::vec2> position2Ds;
    int segmentCount = 40;
    float pi = 3.14159265358979323846f;
    for (int i = 0; i < segmentCount; ++i)
    {
        float angle = (float)i / (float)segmentCount * 2.0f * pi;
        position2Ds.push_back({ 
            center.x + radius * std::cos(angle), 
            center.y + radius * std::sin(angle) });
    }

    return Gizmo::register_.Add(position2Ds, &transform_, color, false).GetID();
}


std::string Gizmo::to_string() const
{
    return logger::make_string("Gizmo on ", transformID ? Entity::GetComponent<Transform>(*transformID).to_string() : " no transform");
}
