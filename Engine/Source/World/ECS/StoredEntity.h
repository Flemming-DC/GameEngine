#pragma once
#include "Naming.h"
#include "Entity.h"


class StoredEntity 
{
public:
	Shorts;
    static Naming naming;

	static void Save(const Entity& entity, string storedName);
	static Entity& Load(const string& entityName); // filename = entityName + .yml
    static Node LoadToNode(uuid storedID);
    //static Entity& Load(uuid id) { return Load(naming.at(id)); }
    //static Node Override(Node overridden, Node overrider);
    //static Node ApplyDifferences(Node node, Node additions, Node removals);
    //static pair<Node, Node> Differences(Node node1, Node node2); // pair< node1 - node2, node2 - node1 > 
    
    static Node Override(const Node& stored, const Node& overrider); // handles modifications and additions to node, but not removals
    static Node GetOverrider(const Node& stored, const Node& combined);


    static Node ToNode(const Entity& in);
    static Entity& FromNode(const Node& node, optional<uuid> instanceID = std::nullopt, 
                            optional<uuid> storedID = std::nullopt, bool _new = false);


};


// ----- conversion extensions ----- 
template<> struct YAML::convert<Entity>
{
    static YAML::Node encode(const Entity& in) // save
    {
        return StoredEntity::ToNode(in);
    }

    static bool decode(const YAML::Node& node, Entity& out) // load
    {
        out = StoredEntity::FromNode(node);
        return true;
    }
};

