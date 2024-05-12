#include "StoredEntity.h"
#include "Component.h"
#include "EngineLiterals.h"
#include "NamingSaver.h"

Shorts;
Naming StoredEntity::naming;


void StoredEntity::Save(const Entity& entity, string storedName)
{
    string path = Literals::Entities + storedName + ".yml";
    uuid storedID = entity.GetStoredID() ? *entity.GetStoredID() : UuidCreator::MakeID();
    naming.Overwrite(storedName, storedID);
    NamingSaver::Save();
    Node node = ToNode(entity);
    node.remove("name");
    node.remove("storedID");
    YmlTools::Save(node, path, true, true);
}

Entity& StoredEntity::Load(const string& entityName) 
{
    uuid storedID = naming.at(entityName);
    Node& node = LoadToNode(storedID);
    if (node["name"] && node["name"].as<string>() != entityName)
        RaiseError("inconsistent naming.");
    node["name"] = entityName;
    return FromNode(node, std::nullopt, storedID, true, true);
}

Node StoredEntity::LoadToNode(uuid storedID)
{
    const string& entityName = naming.at(storedID);
    return YmlTools::Load(Literals::Entities + entityName + ".yml"); 
}


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
    return combined;
}

// combined is the result of applying overrider to stored
Node StoredEntity::GetOverrider(const Node& stored, const Node& combined)
{
    if (!stored.IsMap() || !combined.IsMap())
        RaiseError("Both nodes are expected to be maps.\n",
            "\nstored ", stored, "\ncombined ", combined);

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
    node["name"] = in.Name();
    node["storedID"] = in.GetStoredID();// ? in.GetStoredID() : std::nullopt;
    for (auto& comp : in.GetComponents())
    {
        Node compNode;
        compNode["id"] = comp->GetID();
        comp->Save(compNode); // component-type dependent data
        node[Tools::TypeName(*comp)] = compNode;
    }
    return node;
}

Entity& StoredEntity::FromNode(const Node& node, optional<uuid> instanceID, 
                               optional<uuid> storedID, bool breakIdentity, bool initialize)
{
    if (storedID && !breakIdentity)
        RaiseError("A storedEntity must yield a new Instance when put from Node to Entity.");
    string name = node["name"].as<string>();
    Entity& entity = Entity::register_.Add(name, instanceID, storedID);

    auto componentsMap = node.as<map<string, Node>>();
    for (auto& [compTypeName, compYML] : componentsMap)
    {
        if (!compYML.IsMap())
            continue; // this isnt a component, so we skip it
        if (breakIdentity)
            compYML["id"] = UuidCreator::MakeID(); // this breaks the indentity and makes it a new object

        auto AddComponent = Entity::AddComponentByName.at(compTypeName);
        AddComponent(entity.GetID(), &compYML);
    }

    // initialize components
    if (initialize)
    {
        struct SortByOrder {
            bool operator() (Component* lhs, Component* rhs) const 
                { return lhs->InitOrder() < rhs->InitOrder(); } };

        vector<Component*> comps;
        for (const auto& comp : entity.GetComponents())
            comps.push_back(comp.get());
        std::sort(comps.begin(), comps.end(), SortByOrder());

        for (const auto& comp : comps)
            comp->OnSceneLoaded();
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

