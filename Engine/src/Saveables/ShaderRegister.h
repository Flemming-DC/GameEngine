#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Scene.h"
#include <stduuid/uuid.h>

// rename this to register and template it
class ShaderRegister
{
public:
    static uuids::uuid MakeShader(const std::string& filePath);
    static uuids::uuid Add(const Shader& shader);
    static void Remove(const uuids::uuid& shaderID); // evt. input const Shader& shader
    static const Shader& Get(const uuids::uuid& shaderID);

private:
    static std::vector<Shader> shaders;
    static std::unordered_map<uuids::uuid, int> indexByID;

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

