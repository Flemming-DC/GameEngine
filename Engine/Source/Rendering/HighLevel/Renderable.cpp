#include "Renderable.h"
#include "OpenGlError.h"
#include "EngineAssets.h"
#include "EngineLiterals.h"
#include "Camera.h"
#include "ListTools.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

Shorts;
// static variables must be initialized in the cpp file not the header file
vector<uuid> Renderable::allRenderables;

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
    Assert(wasThere,
        "RenderableID has been removed from allRenderables prematurely");
}


void Renderable::Draw(const mat4& projectionView)
{
    if (!IsFullyEnabled())
        return;
    // if not in view then return;
    Assert(UuidCreator::IsInitialized(material.GetID()) && UuidCreator::IsInitialized(mesh.GetID()),
        "you must setup the mesh and material on the renderable, before drawing it.");
    mat4 model = GetTransform().GetModel(); // this is inefficient
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


bool Renderable::IsInView(const RenderBoundingBox& ViewBounds)
{
    // this checks AABB overlap between two bounding boxes 
    // pos and mesh.MaxExtension() forms the corners of the other box
    vec2 pos = GetTransform().Position2D();
    vec2 size = GetTransform().Scale2D() * mesh.MaxExtension();

    if (ViewBounds.maxX < pos.x - size.x) return false;
    if (ViewBounds.minX > pos.x + size.x) return false;
    if (ViewBounds.maxY < pos.y - size.y) return false;
    if (ViewBounds.minY > pos.y + size.y) return false;

    return true;
}


void Renderable::Save(Node& node) const
{
    node["mesh"] = mesh.GetID();
    
    Node materialNode;
    materialNode["id"] = material.GetID();
    // put properties of builtin materials into node
    auto& name = Material::naming.at(material.GetID());
    if (name == Literals::imageMaterialName)
    {
        materialNode[Literals::u_color] = material.GetUniform<vec4>(Literals::u_color);
        uuid texID = material.GetUniform<Texture*>(Literals::u_textureSampler)->GetID();
        Assert(Texture::register_.Contains(texID),
            "Trying to save material ", name, " on ", to_string(), " with textureID ", texID,
            " that does not exist in the register.");// The textue name is ", Texture::naming.at(texID));
        materialNode[Literals::u_textureSampler] = texID;
    } 
    else if (name == Literals::colorMaterialName)
        materialNode[Literals::u_color] = material.GetUniform<vec4>(Literals::u_color);
    // save the node
    node["material"] = materialNode;


    node["drawOrder"] = drawOrder;
}

void Renderable::Load(const Node& node)
{
    // if nodes don't exist, then we initialize via OnStart
    if (node["material"])
    {
        auto materialNode = node["material"];
        uuid materialID = materialNode["id"].as<uuid>();
        material = Material::register_.Get(materialID); // copy, not ref
        // load properties of builtin materials
        string name = Material::naming.at(materialNode["id"].as<uuid>());
        if (name == Literals::imageMaterialName)
        {
            material.SetUniform(Literals::u_color, materialNode[Literals::u_color].as<vec4>());
            material.SetTexture(Literals::u_textureSampler, materialNode[Literals::u_textureSampler].as<uuid>());
        }
        else if (name == Literals::colorMaterialName)
            material.SetUniform(Literals::u_color, materialNode[Literals::u_color].as<vec4>());
        
    }
    if (node["mesh"])
    {
        uuid meshID = node["mesh"].as<uuid>();
        mesh = Mesh::register_.Get(meshID); // copy, not ref
    }

    drawOrder = node["drawOrder"].as<int>();
}


