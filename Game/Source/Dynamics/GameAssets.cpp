#include "GameAssets.h"
#include "Engine.h"
#include "GameLiterals.h"

Shorts;
static bool isInitialized = false;
static Shader& shader = Shader();
static Texture& texture = Texture();
static Texture& rocketTex = Texture();
static Material& material = Material();


void GameAssets::OnGameStart()
{
    if (isInitialized)
        return;
    isInitialized = true;

    // parameters
    map_uo<string, std::any> uniformsByName = {
        {Literals::u_textureSampler, &texture},
        {Literals::u_color, glm::vec4(0.8f, 0.3f, 0.8f, 1.0f)},
    };

    // assets
    shader = Shader::register_.Add(Literals::imageShader);
    texture = Texture::register_.Add(Literals::Textures + "blizzard attacking fans.png");
    rocketTex = Texture::register_.Add(Literals::Textures + "rocket.png");
    material = Material::register_.Add("mat", shader, uniformsByName);

}

