#include "Scene.h"
#include "ErrorChecker.h"
#include "Entity.h"
#include "Component.h"
#include "StringTools.h"
#include "ListTools.h"
#include "YmlTools.h"
#include "Register.h"
#include "RenderSaver.h"
#include <fstream>
#include <filesystem>
#include <sstream>


Shorts;
using YAML::Node;

unique_ptr<Scene> Scene::activeScene = nullptr;
Event<Scene&> Scene::onStart;
Event<Scene&> Scene::onEnd;

/*
void Scene::Activate(Scene* scenePtr)
{
    Delay::ToFrameEnd([scenePtr]() { ActivateImmediately(scenePtr); });
}
*/


void Scene::SetFirstScene(std::unique_ptr<Scene> firstScene)
{ 
    if (activeScene)
        RaiseError("activeScene is already initialized");
    activeScene = std::move(firstScene);
}

void Scene::ActivateImmediately(Scene* scenePtr)
{
    if (activeScene)
        activeScene->ShutDown();

    activeScene.reset(scenePtr);

    activeScene->Load();
    activeScene->OnStart();
    onStart.Invoke(*activeScene);
}

void Scene::ReloadImmediately()
{
    if (activeScene)
        activeScene->ShutDown();

    //activeScene = std::move(activeScene);

    activeScene->Load();
    activeScene->OnStart();
    onStart.Invoke(*activeScene);
}


void Scene::MakeBlankSceneFile(string name)
{
    // building a scene with a single entity with a transform and a camera
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

    string path = Literals::Scenes + name + ".yml";
    YmlTools::Save(sceneYML, path, false, true);
}

struct SortByOrder
{
    bool operator()(Component* lhs, Component* rhs) const
    {
        return lhs->InitOrder() < rhs->InitOrder();
    }
};

void Scene::Load()
{
    //RenderSaver::Load();
    Node sceneYML = YmlTools::Load(Path());
    id = sceneYML["id"].as<uuid>();

    auto entitiesMap = sceneYML["Entities"].as<map<string, Node>>();
    for (auto& pair : entitiesMap)
    {
        string entityName = pair.first;
        uuid entityID = pair.second["id"].as<uuid>();
        Entity& entity = Entity::register_.Add(entityName, &entityID);

        auto componentsMap = pair.second.as<map<string, Node>>();
        for (auto& [compTypeName, compYML] : componentsMap)
        {
            if (compTypeName == "id")
                continue; // this isnt really a component, so we skip it
            auto AddComponent = Entity::AddComponentByName.at(compTypeName);
            AddComponent(entityID, &compYML);
        }
    }
    for (const auto& entity : Entity::register_.GetData())
    {
        vector<Component*> comps;
        for (const auto& comp : entity.GetComponents())
            comps.push_back(comp.get());
        std::sort(comps.begin(), comps.end(), SortByOrder());
        for (const auto& comp : comps)
            comp->OnSceneLoaded();
    }

}


void Scene::Save()
{
    Scene& scene = *activeScene;
    if (!UuidCreator::IsInitialized(scene.id))
        RaiseError("You are trying to save a scene with an uninitialized id");
        //scene.id = UuidCreator::MakeID();

    RenderSaver::Save();
    Node sceneYML;
    sceneYML["id"] = scene.id;
    Node entitiesYML;
    for (const auto& entity : Entity::register_.GetData())
    {
        Node entityYML;
        entityYML["id"] = entity.GetID();
        for (auto& comp : entity.GetComponents())
        {
            Node compYML;
            compYML["id"] = comp->GetID();
            comp->Save(compYML); // component-type dependent data
            entityYML[Tools::TypeName(*comp)] = compYML;
        }
        entitiesYML[entity.Name()] = entityYML;
    }
    sceneYML["Entities"] = entitiesYML;

    YmlTools::Save(sceneYML, scene.Path(), true, true);
    logger::print("Saved");
}


void Scene::ShutDown() 
{
    Entity::DestroyEverything();
    onEnd.Invoke(*activeScene);
}

