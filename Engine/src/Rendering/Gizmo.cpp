#include "Gizmo.h"
#include "ErrorChecker.h"
#include "Camera.h"
#include "ListTools.h"
#include "UuidCreator.h"
#include "EngineAssets.h"

std::vector<Gizmo*> Gizmo::allGizmos;


Gizmo::Gizmo(std::vector<glm::vec2> position2Ds, Transform* transform_, glm::vec4 color_)
{
    color = color_;
    std::vector<float> positionsRaw;
    for (glm::vec2 position2D : position2Ds)
    {
        positionsRaw.push_back(position2D.x);
        positionsRaw.push_back(position2D.y);
    }

    allGizmos.push_back(this);
    float point_size = 5;
    glCall(glPointSize(point_size));

    material = EngineAssets::GreenGizmoMaterial(); // color unspecified
    mesh.Setup(positionsRaw, {}, { 2, 0, 0, 0 });
    transform = transform_;
    positionCount = position2Ds.size();
}

void Gizmo::ShutDown()
{
    mesh.ShutDown();
    material.ShutDown();
}

Gizmo::~Gizmo()
{
    Tools::Remove(allGizmos, this);
}

void Gizmo::Draw()
{
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



Gizmo* Gizmo::MakeCircle(glm::vec2 center, float radius, Transform* transform_, glm::vec4 color)
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

    return new Gizmo(position2Ds, transform_, color);
}




