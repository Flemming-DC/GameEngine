#include "Scene.h"
#include "ErrorChecker.h"
#include "Entity.h"
#include "Component.h"
#include "StringTools.h"
#include "ListTools.h"
#include "YmlTools.h"
#include "Register.h"
#include "NamingSaver.h"
#include "StoredEntity.h"
#include "Audio.h"
#include <fstream>
#include <filesystem>
#include <sstream>
Shorts;

unique_ptr<Scene> Scene::activeScene = nullptr;
Event<Scene&> Scene::onStart;
Event<Scene&> Scene::onEnd;
Naming Scene::naming;


void Scene::Activate(string name)
{
    Delay::ToFrameEnd([name]() {
        ActivateImmediately(name); });
}

void Scene::ActivateImmediately(string name)
{
    if (activeScene)
        activeScene->ShutDown();

    naming.TryAdd(name, UuidCreator::MakeID());
    uuid id = naming.at(name);
    //naming.Save("sceneNames.yml");
    activeScene.reset(new Scene(id));

    activeScene->Load();
    activeScene->OnStart();
    onStart.Invoke(*activeScene);

}

void Scene::Reload()
{
    Delay::ToFrameEnd([]() { ReloadImmediately(); });
}

void Scene::ReloadImmediately()
{
    if (activeScene)
        activeScene->ShutDown();

    activeScene->Load();
    activeScene->OnStart();
    onStart.Invoke(*activeScene);
}


void Scene::MakeBlankSceneFile(string name)
{
    // handling naming first
    naming.TryAdd(name, UuidCreator::MakeID());
    uuid id = naming.at(name);
    //naming.Save("sceneNames.yml");

    // building a scene with a single entity with a transform and a camera
    Node transformYML;
    transformYML["id"] = UuidCreator::MakeID();
    transformYML["localPosition"] = glm::vec3(0, 0, 0);
    transformYML["localRotation"] = glm::quat(1, 0, 0, 0);
    transformYML["localScale"] = glm::vec3(1, 1, 1);

    Node cameraYML;
    cameraYML["id"] = UuidCreator::MakeID();

    Node cameraEntityYML;
    cameraEntityYML["name"] = "camera";// = UuidCreator::MakeID();
    cameraEntityYML["Transform"] = transformYML;
    cameraEntityYML["Camera"] = cameraYML;

    Node entitiesYML;
    entitiesYML[naming.at(name)] = cameraEntityYML;

    Node sceneYML;
    sceneYML["Entities"] = entitiesYML;

    string path = Literals::Scenes + name + ".yml";
    YmlTools::Save(sceneYML, path, false, true);
}


void Scene::Load()
{
    string path = Literals::Scenes + naming.at(id) + ".yml";
    Node sceneYML = YmlTools::Load(path, true);

    auto storedEntitiesMap = sceneYML["StoredEntities"].IsMap() ? sceneYML["StoredEntities"].as<map<uuid, Node>>() : map<uuid, Node>();
    auto entitiesMap = sceneYML["Entities"].IsMap() ? sceneYML["Entities"].as<map<uuid, Node>>() : map<uuid, Node>();

    for (auto& [entityID, overriderYML] : storedEntitiesMap)
    {
        uuid storedID = overriderYML["storedID"].as<uuid>();
        string name = StoredEntity::naming.at(storedID);
        Node stored = StoredEntity::LoadToNode(storedID);
        Node combined = StoredEntity::Override(stored, overriderYML);
        StoredEntity::FromNode(combined, entityID, storedID, false, false);
    }
    for (auto& [entityID, entityYML] : entitiesMap)
    {
        StoredEntity::FromNode(entityYML, entityID, std::nullopt, false, false);
    }

    // ------- initialize components -------
    
    struct SortByOrder {
        bool operator() (Component* lhs, Component* rhs) const 
            { return lhs->InitOrder() < rhs->InitOrder(); } };
    
    for (const auto& entity : Entity::register_.GetData())
    {
        vector<Component*> comps;
        for (const auto& comp : entity.GetComponents())
            comps.push_back(comp.get());
        std::sort(comps.begin(), comps.end(), SortByOrder());


        for (const auto& comp : comps)
        {
            comp->OnSceneLoaded();
        }
    }
    
}


void Scene::Save()
{
    Scene& scene = *activeScene;
    if (!UuidCreator::IsInitialized(scene.id))
        RaiseError("You are trying to save a scene with an uninitialized id");

    NamingSaver::Save();
    Node sceneYML;
    Node storedEntitiesYML;
    Node entitiesYML;
    for (const auto& entity : Entity::register_.GetData())
    {
        bool isStored = entity.GetStoredID().has_value(); //StoredEntity::naming.Contains(entity.GetID());
        if (isStored)
        {
            Node stored = StoredEntity::LoadToNode(*entity.GetStoredID()); // this is the unchanged / stored part of entity
            Node combined = StoredEntity::ToNode(entity); // this is the runtime version of the entity
            storedEntitiesYML[entity.GetID()] = StoredEntity::GetOverrider(stored, combined); // runtime minus stored = overrider
        }
        else
            entitiesYML[entity.GetID()] = StoredEntity::ToNode(entity);
        
    }
    sceneYML["StoredEntities"] = storedEntitiesYML;
    sceneYML["Entities"] = entitiesYML;

    string path = Literals::Scenes + scene.naming.at(scene.id) + ".yml";
    YmlTools::Save(sceneYML, path, true, true);
    logger::print("Saved");
}


void Scene::ShutDown() 
{
    Delay::OnSceneEnd();
    Entity::DestroyEverything();
    onEnd.Invoke(*activeScene);
}

