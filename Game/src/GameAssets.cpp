#include "GameAssets.h"
#include "Engine.h"


//Shader GameAssets::shader = Shader::register_.Add();
Shader GameAssets::shader;
Texture GameAssets::texture;
Material GameAssets::material;


void GameAssets::Setup()
{
    Log("GameAssets::Setup");
    // parameters
    glm::vec4 color = { 0.8f, 0.3f, 0.8f, 1.0f };
    std::map<std::string, std::any> uniformsByName = {
        {"u_textureSampler", &texture},
        {"u_color", color},
        {"u_MVP", Material::MissingUniform()}
    };

    // assets
    shader.Setup("res/shaders/Image.shader");
    texture.Setup("res/textures/blizzard attacking fans.png");
    material.Setup(shader, {
        {"u_textureSampler", &texture},
        {"u_color", glm::vec4( 0.8f, 0.3f, 0.8f, 1.0f )},
        {"u_MVP", Material::MissingUniform()}
        });
}

