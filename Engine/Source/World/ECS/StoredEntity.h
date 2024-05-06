#pragma once
#include "Naming.h"
#include "Entity.h"


class StoredEntity 
{
public:
	Shorts;
    static Naming naming;

	static void Save(const Entity& entity);
	static Entity& Load(const string& entityName); // filename = entityName + .yml
    //static Entity& Load(uuid id) { return Load(naming.at(id)); }

    static YAML::Node ToNode(const Entity& in);
    static Entity& FromNode(const YAML::Node& node, const string& name);


};


// ----- conversion extensions ----- 
namespace YAML
{
    template<>
    struct convert<Entity>
    {
        static YAML::Node encode(const Entity& in) // save
        {
            return StoredEntity::ToNode(in);
        }

        static bool decode(const YAML::Node& node, Entity& out) // load
        {
            out = StoredEntity::FromNode(node, "Entity");
            return true;
        }
    };

}