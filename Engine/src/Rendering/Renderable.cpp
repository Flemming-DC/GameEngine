#include "Renderable.h"
#include "Camera.h"
#include "ErrorChecker.h"
#include "ListTools.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// static variables must be initialized in the cpp file not the header file
std::vector<Renderable*> Renderable::allRenderables; 

void Renderable::OnConstructed()
{
    allRenderables.push_back(this);
}

void Renderable::OnDestroyed()
{
	Tools::Remove(allRenderables, this);
}

void Renderable::SetByInspector(const Mesh& mesh_, const Material& material_)
{
    mesh = mesh_; // copy, not ref
    material = material_; // copy, not ref
}



void Renderable::Draw()
{
    glm::mat4 projection = Camera::GetCurrent()->GetProjection(); // evt. save the projectionView. at least within each frame
    glm::mat4 view = Camera::GetCurrent()->GetView();
    glm::mat4 model = GetTransform().GetModel(); // this is inefficient
    material.SetUniform("u_MVP", projection * view * model);

    material.Bind();
    mesh.Bind();

    // GL_TRIANGLES should be soft coded if we want to support other things.
    glCall(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
}


void Renderable::UnBind()
{
    Mesh::UnBind();
    Material::UnBind();
}

/*
Mesh& operator=(const Mesh& other) // use if introducing ref members
{
    if (this != &other) // Check for self-assignment
    {
        vertexBuffer = other.vertexBuffer; // Copy or assign each member
        vertexArray = other.vertexArray;
        indexBuffer = other.indexBuffer;
    }
    return *this;
}*/

