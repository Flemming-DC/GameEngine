#pragma once
#include "Engine.h"
#include "InputAction.h"
#include "Entity.h"
//#include "CompYML.h"


class DummyComp : public Component
{
public:

private:
	unsigned int onConnectedChangedIndex = -1;
	//std::vector<DemoScene> demoScenes;
	//std::vector<SecondScene> secondScene;

	void OnStart() override;
	void OnUpdate() override;
	void OnDestroy() override;

	void OldTests();
	void TestInputActions();
};
//compYML(DummyComp);




/*
template<>
struct convert<DummyComp>
{
    static Node encode(const DummyComp& in) // save
    {
        Node node;
        node["id"] = in.GetID();
        in.Save(node); // component-type dependent data
        node["compType"] = Tools::TypeName(in);
        node["entityID"] = in.Entity().GetID();
        return node;
    }

    static bool decode(const Node& node, DummyComp& out) // load
    {
        if (!node.IsMap())
            return false;
        uuids::uuid entityID = node["entityID"].as<uuids::uuid>();
        auto compType = node["compType"].as<std::string>();
        if (compType != Tools::TypeName<DummyComp>())
            return false;
        Entity::Add<DummyComp>(entityID);
        return true;
    }
};

template<>
struct convert<DummyComp>
{
    static Node encode(const DummyComp& in) { return Node(); }

    static bool decode(const Node& node, DummyComp& out) // load
    {
        if (!node.IsMap())
            return false;
        auto compType = node["compType"].as<std::string>();
        return compType == Tools::TypeName<DummyComp>();
    }
};

*/