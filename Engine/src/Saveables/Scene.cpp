#include "Scene.h"
#include "ErrorChecker.h"
#include "Entity.h"
#include "Component.h"
#include "StringTools.h"
#include "ListTools.h"
#define YAML_CPP_STATIC_DEFINE // put evt. denne macro i en dedikeret Asset class eller YML class
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <filesystem>
#include <sstream>

// decode(const Node& node, uuid& out) skal udvides sådan at vis en id mangler så oprettes
// den og en warning logges. Hvis den er ugyldig så rejses en fejl.


using namespace std; 
using namespace uuids; 
using namespace YAML;

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

    auto entitiesMap = sceneYML["Entities"].as<map<string, Node>>();
    for (auto& pair : entitiesMap)
    {
        string entityName = pair.first;
        uuid entityID = pair.second["id"].as<uuid>();
        // make entity with fixed id (stored in register)

        auto componentsMap = pair.second.as<map<string, Node>>();
        Tools::RemoveKey(componentsMap, (string)"id");
        for (auto& pair_ : componentsMap)
        {
            string compTypeName = pair_.first;
            uuid compID = pair_.second["id"].as<uuid>();
            // component-type dependent data

            // add component width fixed id
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
            // component-type dependent data
            entityYML[Tools::to_string(*comp)] = compYML;
        }
        entitiesYML[entity.name] = entityYML;
    }
    sceneYML["Entities"] = entitiesYML;


    // Serialize the data to a YAML file
    ofstream outStream(Path());
    outStream << sceneYML;
    outStream.close();

}



// use this or not??, if yes don't put it here
namespace YAML
{
    template<>
    struct convert<uuid>
    {
        static Node encode(const uuid& in)
        {
            Node node;
            node = UuidCreator::to_string(in);
            return node;
        }

        static bool decode(const Node& node, uuid& out)
        {
            if (!node.IsScalar())
                return false;
            out = UuidCreator::from_string(node.as<string>());
            return true;
        }
    };
}

