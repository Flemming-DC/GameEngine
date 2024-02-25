#include "Scene.h"
#include "ErrorChecker.h"
#include "Entity.h"
#include "Component.h"
#include "StringTools.h"
#include "ListTools.h"
#include "YAML.h"
#include "Transform.h" // temp
#include "Camera.h" // temp
#include "Renderable.h" // temp
#include "PolygonCollider.h" // temp
#include "RectangleCollider.h" // temp
#include "CircleCollider.h" // temp
#include <fstream>
#include <filesystem>
#include <sstream>

// decode(const Node& node, uuid& out) skal udvides sådan at vis en id mangler så oprettes
// den og en warning logges. Hvis den er ugyldig så rejses en fejl.
// loading a scene should eliminate the previous scene
// raise callbacks or events onSceneStart and onSceneEnd

using namespace std; 
using namespace uuids; 
using namespace YAML;


//static Register<Scene> register_;

void Scene::Setup() 
{
    if (UuidCreator::IsInitialized(id))
        RaiseError("Scene is already initialized");
    id = UuidCreator::MakeID();
	//entityIDs = PurelyManualSetup();
	Load();
    ManualSetup();
}


void Scene::Load()
{
    if (!filesystem::exists(Path()))
        RaiseError("Cannot load Scene at " + Path() + ", since there is no Scene there.");

    Node sceneYML = LoadFile(Path());
    id = sceneYML["id"].as<uuid>();

    auto entitiesMap = sceneYML["Entities"].as<map<string, Node>>();
    for (auto& pair : entitiesMap)
    {
        string entityName = pair.first;
        uuid entityID = pair.second["id"].as<uuid>();
        Entity& entity = Entity::register_.Add(entityName, &entityID);

        auto componentsMap = pair.second.as<map<string, Node>>();
        for (auto& pair_ : componentsMap)
        {
            string compTypeName = pair_.first;
            Node compYML = pair_.second;

            if (compTypeName == "Transform")
                entity.LoadComponent<Transform>(compYML);
            else if (compTypeName == "Camera")
                entity.LoadComponent<Camera>(compYML);
            else if (compTypeName == "Renderable")
                entity.LoadComponent<Renderable>(compYML);
            else if (compTypeName == "PolygonCollider")
                entity.LoadComponent<PolygonCollider>(compYML);
            else if (compTypeName == "RectangleCollider")
                entity.LoadComponent<RectangleCollider>(compYML);
            else if (compTypeName == "CircleCollider")
                entity.LoadComponent<CircleCollider>(compYML);

        }
        entityIDs.push_back(entityID);
    }
    for (const auto& entity : Entity::register_.GetData())
    {
        for (const auto& comp : entity.GetComponents())
            comp->OnSceneLoaded();
    }

}


void Scene::Save()
{
    Node sceneYML;
    sceneYML["id"] = id;
    Node entitiesYML;
    for (auto& entityID : entityIDs)
    {
        Node entityYML;
        entityYML["id"] = entityID;
        auto& entity = Entity::register_.Get(entityID);
        for (auto& comp : entity.GetComponents())
        {
            Node compYML;
            compYML["id"] = comp->GetID();
            comp->Save(compYML); // component-type dependent data
            entityYML[Tools::type_as_string(*comp)] = compYML;
        }
        entitiesYML[entity.name] = entityYML;
    }
    sceneYML["Entities"] = entitiesYML;

    // configure yaml file using emitter
    Emitter emitter;
    emitter.SetIndent(4);
    emitter.SetSeqFormat(YAML::Flow); // write lists horizontally, not vertically
    emitter << sceneYML; 

    // write yaml data to output stream
    ofstream outStream(Path());
    outStream << emitter.c_str();
    outStream.close();

}




