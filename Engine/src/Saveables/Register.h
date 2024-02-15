#pragma once
#include <stduuid/uuid.h>
#include <utility>
#include "ErrorChecker.h"


template<typename DataChunk>
class Register
{
public:

    template<typename... Args>
    const DataChunk& Add(Args&&... args)
    {
        dataChunks.emplace_back(std::forward<Args>(args)...);
        indexByID[dataChunks.back().GetID()] = dataChunks.size() - 1;
        return dataChunks.back();
    }

    const DataChunk& Add()
    {
        dataChunks.emplace_back(DataChunk());
        indexByID[dataChunks.back().GetID()] = dataChunks.size() - 1;
        return dataChunks.back();
    }

    void Remove(const uuids::uuid& id)
    {
        if (!Tools::ContainsKey_unordered(indexByID, id))
            RaiseError("Trying to reomve an object, which isn't in the register.");
        dataChunks.erase(dataChunks.begin() + indexByID[id]);
        Tools::RemoveKey_unordered(indexByID, id);
    }

    const DataChunk& Get(const uuids::uuid& id)
    {
        return dataChunks[indexByID[id]];
    }


private:
    std::vector<DataChunk> dataChunks;
    std::unordered_map<uuids::uuid, int> indexByID;



    //static std::vector<Texture> textures = {};
    //static std::vector<Material> materials = {};
    //static std::vector<Scene> scenes = {};

    /*
    glm::vec4 color = { 0.8f, 0.3f, 0.8f, 1.0f };
    Shader shader("res/shaders/Image.shader");
    Texture texture("res/textures/blizzard attacking fans.png");
    std::map<std::string, std::any> uniformsByName = {
        {"u_textureSampler", &texture},
        {"u_color", color},
        {"u_MVP", Material::MissingUniform()}
    };
    Material material(shader, uniformsByName);
    */
};

