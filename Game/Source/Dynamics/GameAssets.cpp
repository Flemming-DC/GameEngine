#include "GameAssets.h"
#include "Shader.h"
#include "Material.h"
#include "GameLiterals.h"

Shorts;
static bool isInitialized = false;
static Texture& texture = Texture();
static Texture& asteroidTex = Texture();
static Texture& asteroidMessyTex = Texture();
Texture& GameAssets::rocketTex = Texture();
Texture& GameAssets::rocketFlamingTex = Texture();

static Shader& shader = Shader();
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
    texture = Texture::register_.Add(Literals::Textures + "blizzard attacking fans.png");
    rocketTex = Texture::register_.Add(Literals::Textures + "rocket.png");
    rocketFlamingTex = Texture::register_.Add(Literals::Textures + "rocketFlaming.png");
    asteroidTex = Texture::register_.Add(Literals::Textures + "asteroid.png");
    asteroidMessyTex = Texture::register_.Add(Literals::Textures + "asteroidMessy.png");

    shader = Shader::register_.Add(Literals::imageShader);
    material = Material::register_.Add("mat", shader, uniformsByName);

}

