#pragma once
#define YAML_CPP_STATIC_DEFINE // put evt. denne macro i en dedikeret Asset class eller YML class
#include <yaml-cpp/yaml.h>
#include <stduuid/uuid.h>
#include "UuidCreator.h"
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>


namespace YmlTools
{
    YAML::Node Load(std::string path, bool createFileIfAbsent = true);
    void Save(YAML::Node yml, std::string path, bool overwrite, bool horizontalLists);
    bool IsEqual(YAML::Node node1, YAML::Node node2);
}


// ----- conversion extensions ----- 

template<typename T> struct YAML::convert<std::optional<T>>
{
    static Node encode(const std::optional<T>& in) // save
    {
        Node node;
        if (in)
            node = *in;
        return node;
    }

    static bool decode(const Node& node, std::optional<T>& out) // load
    {
        if (!node.IsScalar())
            return false;
        out = node.IsNull() ? std::nullopt : node.as<T>();
        return true;
    }
};

template<> struct YAML::convert<uuids::uuid>
{
    static Node encode(const uuids::uuid& in) // save
    {
        Node node;
        node = logger::to_string(in);
        return node;
    }

    static bool decode(const Node& node, uuids::uuid& out) // load
    {
        if (!node.IsScalar())
            return false;
        out = UuidCreator::from_string(node.as<std::string>());
        return true;
    }
};

template<> struct YAML::convert<glm::vec4>
{
    static Node encode(const glm::vec4& in) // save
    {
        Node node;
        node.push_back(in.r);
        node.push_back(in.g);
        node.push_back(in.b);
        node.push_back(in.a);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& out) // load
    {
        if (!node.IsSequence())
            return false;
        auto vec = (node.as<std::vector<float>>());
        if (vec.size() != 4)
            return false;
        out = { vec[0], vec[1], vec[2], vec[3] };
        return true;
    }
};

template<> struct YAML::convert<glm::vec3>
{
    static Node encode(const glm::vec3& in) // save
    {
        Node node;
        node.push_back(in.x);
        node.push_back(in.y);
        node.push_back(in.z);
        return node;
    }

    static bool decode(const Node& node, glm::vec3& out) // load
    {
        if (!node.IsSequence())
            return false;
        auto vec = (node.as<std::vector<float>>());
        if (vec.size() != 3)
            return false;
        out = { vec[0], vec[1], vec[2] };
        return true;
    }
};


template<> struct YAML::convert<glm::vec2>
{
    static Node encode(const glm::vec2& in) // save
    {
        Node node;
        node.push_back(in.x);
        node.push_back(in.y);
        return node;
    }

    static bool decode(const Node& node, glm::vec2& out) // load
    {
        if (!node.IsSequence())
            return false;
        auto vec = (node.as<std::vector<float>>());
        if (vec.size() != 2)
            return false;
        out = { vec[0], vec[1] };
        return true;
    }
};

template<> struct YAML::convert<glm::quat>
{
    static Node encode(const glm::quat& in) // save
    {
        Node node;
        node.push_back(in.w);
        node.push_back(in.x);
        node.push_back(in.y);
        node.push_back(in.z);
        return node;
    }

    static bool decode(const Node& node, glm::quat& out) // load
    {
        if (!node.IsSequence())
            return false;
        auto vec = (node.as<std::vector<float>>());
        if (vec.size() != 4)
            return false;
        out = { vec[0], vec[1], vec[2], vec[3]};
        return true;
    }
};

