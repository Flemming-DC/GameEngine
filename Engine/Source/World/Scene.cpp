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
#include "Register.h"
#include "Renderer.h"
#include "Gizmo.h"
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
    //Renderer::SetupGrid2D(0.25f); // if is_editor

    activeScene->Load();
    activeScene->ManualSetup();
    onStart.Invoke(*activeScene);
}

void Scene::ReloadImmediately()
{
    if (activeScene)
        activeScene->ShutDown();

    //activeScene = std::move(activeScene);
    //Renderer::SetupGrid2D(0.25f); // if is_editor

    activeScene->Load();
    activeScene->ManualSetup();
    onStart.Invoke(*activeScene);
}


void Scene::MakeBlankSceneFile(string name)
{
    string path = "res/Scenes/" + name + ".yml";
    if (std::filesystem::exists(path))
        RaiseError("Cannot create Scene at " + path + ", since there is already a Scene there.");

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


    // configure yaml file using emitter
    YAML::Emitter emitter;
    emitter.SetIndent(4);
    emitter.SetSeqFormat(YAML::Flow); // write lists horizontally, not vertically
    emitter << sceneYML;

    // write yaml data to output stream
    std::ofstream outStream(path);
    outStream << emitter.c_str();
    outStream.close();
}

void Scene::Load()
{
    if (!std::filesystem::exists(Path()))
        RaiseError("Cannot load Scene at " + Path() + ", since there is no Scene there.");

    Node sceneYML = YAML::LoadFile(Path());
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
    }
    for (const auto& entity : Entity::register_.GetData())
    {
        for (const auto& comp : entity.GetComponents())
            comp->OnSceneLoaded();
    }

}


void Scene::Save()
{
    Scene& scene = *activeScene;
    if (!UuidCreator::IsInitialized(scene.id))
        RaiseError("You are trying to save a scene with an uninitialized id");
        //scene.id = UuidCreator::MakeID();

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
        entitiesYML[entity.GetName()] = entityYML;
    }
    sceneYML["Entities"] = entitiesYML;

    // configure yaml file using emitter
    YAML::Emitter emitter;
    emitter.SetIndent(4);
    emitter.SetSeqFormat(YAML::Flow); // write lists horizontally, not vertically
    emitter << sceneYML; 

    // write yaml data to output stream
    std::ofstream outStream(scene.Path());
    outStream << emitter.c_str();
    outStream.close();

}


void Scene::ShutDown() 
{
    Entity::DestroyEverything();
    Gizmo::Clear();
    onEnd.Invoke(*activeScene);
}

