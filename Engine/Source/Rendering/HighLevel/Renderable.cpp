#include "Renderable.h"
#include "OpenGlError.h"
#include "EngineAssets.h"
#include "ListTools.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


// static variables must be initialized in the cpp file not the header file
std::vector<uuids::uuid> Renderable::allRenderables;

void Renderable::OnStart()
{
    allRenderables.push_back(GetID());
}

void Renderable::OnDestroy()
{
	bool wasThere = Tools::Remove(allRenderables, GetID());
    if (!wasThere)
        RaiseError("RenderableID has been removed from allRenderables prematurely");
}

void Renderable::Setup(const Material& material_, const Mesh& mesh_)
{
    material = material_; // copy, not ref
    mesh = mesh_; // copy, not ref
}
void Renderable::Setup(const Material& material_)
{
    material = material_; // copy, not ref
    mesh = EngineAssets::SquareMesh(); // copy, not ref

}



void Renderable::Draw(const glm::mat4& projectionView)
{
    glm::mat4 model = GetTransform().GetModel(); // this is inefficient
    material.SetUniform("u_MVP", projectionView * model);

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


void Renderable::Save(YAML::Node& node) const
{
    if (mesh.GetID() != EngineAssets::SquareMesh().GetID())
        node["mesh"] = mesh.GetID();
    node["material"] = material.GetID();

}

void Renderable::Load(const YAML::Node& node)
{
    //auto materialID = node["material"].as<uuids::uuid>(); // material cannot yet be loaded, since it aren't saved (in the current version)
    //material = Material::register_.Get(materialID); // copy, not ref

    if (node["mesh"])
    {
        auto meshID = node["mesh"].as<uuids::uuid>();
        mesh = Mesh::register_.Get(meshID); // copy, not ref
    }
    else
    {
        mesh = EngineAssets::SquareMesh(); // copy, not ref
    }
}

