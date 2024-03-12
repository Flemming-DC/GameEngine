#include "Scene.h"
#include "ErrorChecker.h"
#include "Entity.h"
#include "Component.h"
#include "StringTools.h"
#include "ListTools.h"
#include "YAML.h"
#include "Transform.h" 
#include "Camera.h" 
#include "Renderable.h" 
#include "PolygonCollider.h" 
#include "RectangleCollider.h" 
#include "CircleCollider.h" 
#include <fstream>
#include <filesystem>
#include <sstream>

// decode(const Node& node, uuid& out) skal udvides sådan at vis en id mangler så oprettes
// den og en warning logges. Hvis den er ugyldig så rejses en fejl.
// raise callbacks or events onSceneStart and onSceneEnd

using namespace std; 
using namespace uuids; 
using namespace YAML;

std::unique_ptr<Scene> Scene::activeScene = nullptr;
Event<Scene&> Scene::onStart;
Event<Scene&> Scene::onEnd;


void Scene::MakeBlankSceneFile(string name)
{
    string path = "res/Scenes/" + name + ".yml";
    if (filesystem::exists(path))
        RaiseError("Cannot create Scene at " + path + ", since there is already a Scene there.");

    Node transformYML;
    transformYML["id"] = UuidCreator::MakeID();
    transformYML["localPosition"] = glm::vec3(0, 0, 0);
    transformYML["localRotation"] = glm::quat(1, 0, 0, 0);
    transformYML["localScale"] = glm::vec3(1, 1, 1);

    Node cameraYML;
    cameraYML["id"] = UuidCreator::MakeID();

    Node cameraEntityYML;
    cameraEntityYML["id"] = UuidCreator::MakeID();
    cameraEntityYML["Transform"] = transformYML;
    cameraEntityYML["Camera"] = cameraYML;

    Node entitiesYML;
    entitiesYML["camera"] = cameraEntityYML;

    Node sceneYML;
    sceneYML["id"] = UuidCreator::MakeID();
    sceneYML["Entities"] = entitiesYML;


    // configure yaml file using emitter
    Emitter emitter;
    emitter.SetIndent(4);
    emitter.SetSeqFormat(YAML::Flow); // write lists horizontally, not vertically
    emitter << sceneYML;

    // write yaml data to output stream
    ofstream outStream(path);
    outStream << emitter.c_str();
    outStream.close();
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
    if (!UuidCreator::IsInitialized(id))
        id = UuidCreator::MakeID();

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
            entityYML[Tools::TypeName(*comp)] = compYML;
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


void Scene::ShutDown() 
{
    for (const auto& entityID : entityIDs)
    {
        Entity::register_.Get(entityID).Destroy();
        //Entity::register_.Remove(entityID); //<--- this is missing
    }
    Entity::register_.Clear();
    Gizmo::register_.Clear();
    onEnd.Invoke(*activeScene);
}

