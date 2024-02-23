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

struct ReverseStringComparator
{
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return lhs > rhs; // Order strings in reverse
    }
};

//static Register<Scene> register_;

void Scene::Setup(string name_) 
{
    if (UuidCreator::IsInitialized(id))
        RaiseError("Scene is already initialized");
    id = UuidCreator::MakeID();
	name = name_;
	entityIDs = MakeEntities();
	//Load();
}

void Scene::Load()
{
    if (!filesystem::exists(Path()))
        RaiseError("Cannot load Scene at " + Path() + ", since there is no Scene there.");

    Node sceneYML = LoadFile(Path());
    id = sceneYML["id"].as<uuid>();

    Node entityNode = sceneYML["Entities"];
    auto entitiesMap = entityNode.as<map<string, Node>>(); // this randomly gave a bug? weird!
    //auto entitiesMap = sceneYML["Entities"].as<map<string, Node>>(); // this randomly gave a bug? weird!
    for (auto& pair : entitiesMap)
    {
        string entityName = pair.first;
        uuid entityID = pair.second["id"].as<uuid>();
        Entity& entity = Entity::Load(entityID, entityName);

        auto componentsMap = pair.second.as<map<string, Node, ReverseStringComparator>>();
        //std::sort(std::begin(componentsMap), std::end(componentsMap), );
        //Tools::RemoveKey(componentsMap, (string)"id");
        Log("HUSK AT HÅNDTERE COMPONENT RÆKKEFØLGEN ORDENLIGT");
        for (auto& pair_ : componentsMap)
        {
            string compTypeName = pair_.first;
            Log(compTypeName);
            Node compYML = pair_.second;
            //uuid compID = pair_.second["id"].as<uuid>();

            // There is an annoying ordering dependence in Transform vs. Camera
            // The custom logic cannot be included in this way, since the Engine mustn't refer to the Game
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
            else
                Warning("Unrecognized componentType: " + compTypeName + " . This component cannot be loaded.");

            // component-type dependent data

            // add component width fixed id
            // entity.LoadComponent<>(); // compType from compTypeName
        }
        entityIDs.push_back(entityID);
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
            entityYML[Tools::to_string(*comp)] = compYML;
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




