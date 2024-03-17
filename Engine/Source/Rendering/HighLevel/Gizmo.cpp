#include "Gizmo.h"
#include "OpenGlError.h"
#include "Camera.h"
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

    std::vector<float> positionsRaw;
    for (glm::vec2 position2D : position2Ds)
    {
        positionsRaw.push_back(position2D.x);
        positionsRaw.push_back(position2D.y);
    }

    float point_size = 5;
    glCall(glPointSize(point_size));

    material = EngineAssets::GreenGizmoMaterial(); // color unspecified
    mesh = Mesh::register_.Add(positionsRaw, std::vector<unsigned int>(), VertexLayout({ 2, 0, 0, 0 }));
    transform = transform_;
    positionCount = position2Ds.size();
}



void Gizmo::Draw()
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("Cannot draw uninitialized gizmo");
    glm::mat4 projection = Camera::GetCurrent()->GetProjection(); // evt. save the projectionView. at least within each frame
    glm::mat4 view = Camera::GetCurrent()->GetView();
    glm::mat4 model = transform != nullptr ? transform->GetModel() : glm::mat4(1.0f); // this is inefficient
    material.SetUniform("u_MVP", projection * view * model);
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
    return logger::make_string("Gizmo on ", transform ? transform->to_string() : "no transform");
}
