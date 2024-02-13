#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Scene.h"
#include <stduuid/uuid.h>
#include <utility>

// rename this to register and template it
template<typename T>
class Register
{
public:

    template<typename... Args>
    uuids::uuid Make(Args&&... args)
    {
        ts.emplace_back(std::forward<Args>(args)...);
        indexByID[ts.back().GetID()] = ts.size() - 1;
        return ts.back().GetID();
    }

    void Remove(const uuids::uuid& id)
    {
        if (!Tools::ContainsKey_unordered(indexByID, id))
            RaiseError("Trying to reomve an object, which isn't in the register.");
        ts.erase(ts.begin() + indexByID[id]);
        Tools::RemoveKey_unordered(indexByID, id);
    }

    const T& Get(const uuids::uuid& id)
    {
        return ts[indexByID[id]];
    }


private:
    std::vector<T> ts;
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

