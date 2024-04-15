#include "Gizmo.h"
#include "OpenGlError.h"
#include "ListTools.h"
#include "UuidCreator.h"
#include "EngineAssets.h"
#include "EngineMode.h"

Shorts;
Register<Gizmo> Gizmo::register_;



Gizmo::Gizmo(vector<vec2> position2Ds, Transform* transform_, vec4 color_, bool showPoints_, bool loop_)
{
    if (UuidCreator::IsInitialized(id))
        RaiseError("Gizmo is already initialized");
    id = UuidCreator::MakeID();

    color = color_;
    showPoints = showPoints_;
    loop = loop_;
    if (transform_)
        transformID = transform_->GetID();

    vector<float> positionsRaw;
    for (vec2 position2D : position2Ds)
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



void Gizmo::Draw(const mat4& projectionView)
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("Cannot draw uninitialized gizmo");
    mat4 model = transformID ? Entity::GetComponent<Transform>(*transformID).GetModel() : mat4(1.0f);
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
    vector<uuid> tobeRemoved;
    for (Gizmo& gizmo : Gizmo::register_.GetData())
    {
        if (gizmo.transformID && !Entity::TryGetComponent<Transform>(*gizmo.transformID))
            tobeRemoved.push_back(gizmo.GetID());
    }
    for (uuid& gizmoID : tobeRemoved)
        Gizmo::register_.Remove(gizmoID);

}


uuid Gizmo::Make(vector<vec2> position2Ds, Transform* transform, vec4 color, bool showPoints, bool loop)
{
    uuid nullID;
    if (!EngineMode::InEditor())
        return nullID; // no Gizmos outsize editor
    return register_.Add(position2Ds, transform, color, showPoints, loop).GetID();
}

uuid Gizmo::MakeCircle(vec2 center, float radius, Transform& transform_, vec4 color)
{
    vector<vec2> position2Ds;
    int segmentCount = 40;
    float pi = 3.14159265358979323846f;
    for (int i = 0; i < segmentCount; ++i)
    {
        float angle = (float)i / (float)segmentCount * 2.0f * pi;
        position2Ds.push_back({ 
            center.x + radius * std::cos(angle), 
            center.y + radius * std::sin(angle) });
    }

    return Gizmo::Make(position2Ds, &transform_, color, false);
}


string Gizmo::to_string() const
{
    return logger::make_string("Gizmo on ", transformID ? Entity::GetComponent<Transform>(*transformID).to_string() : " no transform");
}
