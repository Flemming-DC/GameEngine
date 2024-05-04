#pragma once
#include "Entity.h"
#include "Component.h"
#include "YmlTools.h"

#define compYML(compType_) \
namespace YAML \
{ \
    template<> \
    struct convert<compType_> \
    { \
        static Node encode(const compType_& in) \
        { \
            Node node; \
            node["id"] = in.GetID(); \
            in.Save(node); \
            node["compType"] = Tools::TypeName(in); \
            node["entityID"] = in.Entity().GetID(); \
            return node; \
        } \
 \
        static bool decode(const Node& node, compType_& out) \
        { \
            if (!node.IsMap()) \
                return false; \
            uuids::uuid entityID = node["entityID"].as<uuids::uuid>(); \
            auto compType = node["compType"].as<std::string>(); \
            if (compType != Tools::TypeName<compType_>()) \
                return false; \
            Entity::Add<compType_>(entityID); \
            return true; \
        } \
    }; \
} 


/*
class DummyComp;

namespace YAML
{

    template<>
    struct convert<DummyComp>
    {
        static Node encode(const DummyComp& in) // save
        {
            Node node;
            //node["id"] = in.GetID();
            //in.Save(node); // component-type dependent data
            node["compType"] = Tools::TypeName(in);
            //node["entityID"] = in.Entity().GetID();
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
}
*/


