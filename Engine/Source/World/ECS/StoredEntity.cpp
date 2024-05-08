#include "StoredEntity.h"
#include "Component.h"
#include "EngineLiterals.h"

Shorts;

Naming StoredEntity::naming;

void StoredEntity::Save(const Entity& entity)
{
    string path = Literals::Entities + entity.Name() + ".yml";
    naming.Overwrite(path, entity.GetID());
    YmlTools::Save(ToNode(entity), entity.Name(), true, true);
}

Entity& StoredEntity::Load(const string& entityName) {
    return FromNode(LoadToNode(entityName), entityName); }

Node StoredEntity::LoadToNode(const string& entityName) { 
    return YmlTools::Load(Literals::Entities + entityName + ".yml"); }


Node StoredEntity::Override(const Node& stored, const Node& overrider)
{
    if (!overrider.IsDefined() || overrider.IsNull())
        return stored;
    if (!overrider.IsMap())
        RaiseError("Node is defined, not null and not a map. This is unexpected.");

    Node combined = stored;
    auto overriderMap = overrider.as<map<string, Node>>();
    for (const auto& [key, value] : overriderMap)
    {
        if (value.IsMap())
            combined[key] = Override(stored[key], value);
        else
            combined[key] = value;
    }
    P("-------- combined --------\n");
    P(combined);
    return combined;
}

// combined is the result of applying overrider to stored
Node StoredEntity::GetOverrider(const Node& stored, const Node& combined)
{
    if (!stored.IsMap() || !combined.IsMap())
        RaiseError("Both nodes are expected to be maps.");

    Node overrider;
    auto combinedMap = combined.as<map<string, Node>>();
    for (const auto& pair : combinedMap)
    {
        const string& key = pair.first;
        const Node& value = pair.second;

        if (!stored[key])
            overrider[key] = value;
        else if (YmlTools::IsEqual(stored[key], value))
            continue;
        else if (!value.IsMap())
            overrider[key] = value;
        else
        {
            Node n = GetOverrider(stored[key], value);
            if (!n.IsNull())
                overrider[key] = n;
        }

    }
    return overrider;

}


Node StoredEntity::ToNode(const Entity& in)
{
    Node node;
    node["id"] = in.GetID();
    for (auto& comp : in.GetComponents())
    {
        Node compNode;
        compNode["id"] = comp->GetID();
        comp->Save(compNode); // component-type dependent data
        node[Tools::TypeName(*comp)] = compNode;
    }
    return node;
}

Entity& StoredEntity::FromNode(const Node& node, const string& name)
{
    uuid entityID = node["id"].as<uuid>();
    Entity& entity = Entity::register_.Add(name, &entityID);

    auto componentsMap = node.as<map<string, Node>>();
    for (auto& [compTypeName, compYML] : componentsMap)
    {
        if (compTypeName == "id")
            continue; // this isnt really a component, so we skip it
        auto AddComponent = Entity::AddComponentByName.at(compTypeName);
        AddComponent(entityID, &compYML);
    }
    return entity;
}



/*
map_uo<se, std::function<uuid(vec2, string)>> makeBySE;

uuid MakeSprite(vec2 position, string name = "Sprite")
{
    Entity& entity = Entity::Make<Renderable, RectangleCollider>(name);
    //entity.Get<Renderable>().se

    return Entity::Make(name)
        .Get<Transform>()
            .SetPosition2D(position)
        .Add<Renderable>()
            .SetMaterial(Literals::myMat)
        .Add<RectangleCollider>()
            .SetSize(vec2(2.0f, 3.0f))
        .Add<Health>()
            .SetMaxHealth(60)
    
    makeBySE[se::Sprite] = MakeSprite; // shouldn't be in this place


    return entity.GetID();
    //return entity;
}
*/

