#include "GameAssets.h"
#include "Engine.h"


Shader& GameAssets::shader = Shader();
Texture& GameAssets::texture = Texture();
Material& GameAssets::material = Material();


void GameAssets::Setup()
{
    Log("GameAssets::Setup");
    // parameters
    std::map<std::string, std::any> uniformsByName = {
        {"u_textureSampler", &texture},
        {"u_color", glm::vec4(0.8f, 0.3f, 0.8f, 1.0f)},
        {"u_MVP", Material::MissingUniform()}
    };

    // assets
    shader = Shader::register_.Add("res/shaders/Image.shader");
    texture = Texture::register_.Add("res/textures/blizzard attacking fans.png");
    material = Material::register_.Add(shader, uniformsByName);
}

