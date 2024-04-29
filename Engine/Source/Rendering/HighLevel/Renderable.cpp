#include "Renderable.h"
#include "OpenGlError.h"
#include "EngineAssets.h"
#include "ListTools.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shorts;
using YAML::Node;
// static variables must be initialized in the cpp file not the header file
std::vector<uuids::uuid> Renderable::allRenderables;

void Renderable::OnStart()
{
    allRenderables.push_back(GetID());
    if (!UuidCreator::IsInitialized(material.GetID()))
        material = EngineAssets::DefaultMaterial(); // copy, not ref
    if (!UuidCreator::IsInitialized(mesh.GetID()))
        mesh = EngineAssets::SquareMesh(); // copy, not ref
}

void Renderable::OnDestroy()
{
	bool wasThere = Tools::Remove(allRenderables, GetID());
    if (!wasThere)
        RaiseError("RenderableID has been removed from allRenderables prematurely");
}




void Renderable::Draw(const glm::mat4& projectionView)
{
    if (!UuidCreator::IsInitialized(material.GetID()) || !UuidCreator::IsInitialized(mesh.GetID()))
        RaiseError("you must setup the mesh and material on the renderable, before drawing it.");
        
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
    node["mesh"] = Mesh::naming.at(mesh.GetID());
    //node["material"] = Material::naming.at(material.GetID());
    
    Node materialNode;
    auto& name = Material::naming.at(material.GetID());
    materialNode["name"] = name; // evt. save id too
    if (name == "image")
    {
        materialNode["u_color"] = material.GetUniform<vec4>("u_color");
        uuid texID = material.GetUniform<Texture*>("u_textureSampler")->GetID();
        materialNode["u_textureSampler"] = Texture::naming.at(texID);
    }
    node["material"] = materialNode;
    
}

void Renderable::Load(const YAML::Node& node)
{
    // if nodes don't exist, then we initialize via OnStart
    if (node["material"])
    {
        auto materialNode = node["material"];
        uuid materialID = Material::naming.at(materialNode["name"].as<string>());
        material = Material::register_.Get(materialID); // copy, not ref
        if (materialNode["u_color"])
            material.SetUniform("u_color", materialNode["u_color"].as<vec4>());
        if (materialNode["u_textureSampler"])
            material.SetTexture("u_textureSampler", materialNode["u_textureSampler"].as<string>());
    }
    if (node["mesh"])
    {
        uuid meshID = Mesh::naming.at(node["mesh"].as<string>());
        mesh = Mesh::register_.Get(meshID); // copy, not ref
    }

}

