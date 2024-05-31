#include "GameAssets.h"
#include "Shader.h"
#include "Material.h"
#include "GameLiterals.h"

Shorts;

#define MakeTexture(tex) static Texture& tex = _MakeTexture(#tex);
#define SetupTexture(tex) _SetupTexture(tex, #tex);
Texture& _MakeTexture(const char* name)
{
    return Texture::register_.Add(Literals::Textures + name + ".png");
}
void _SetupTexture(Texture& tex, const char* name)
{
    tex = Texture::register_.Add(Literals::Textures + name + ".png");
}

Texture& GameAssets::rocket = Texture();
Texture& GameAssets::rocketFlaming = Texture();
Texture& GameAssets::explosion = Texture();
static Texture& blizzardAttackingFans = Texture();


void GameAssets::OnGameStart()
{
    static bool isInitialized = false;
    if (isInitialized)
        return;
    isInitialized = true;

    SetupTexture(rocket);
    SetupTexture(rocketFlaming);
    SetupTexture(blizzardAttackingFans);
    SetupTexture(explosion);

    MakeTexture(asteroid);
    MakeTexture(asteroidMessy);
    MakeTexture(asteroid3);
    MakeTexture(exhaust);
    MakeTexture(longSteelbar);
    MakeTexture(rock);
    MakeTexture(steelbar);
    MakeTexture(stellarBackground);

    // parameters
    map_uo<string, std::any> uniformsByName = {
        {Literals::u_textureSampler, &blizzardAttackingFans},
        {Literals::u_color, glm::vec4(0.8f, 0.3f, 0.8f, 1.0f)},
    };

    static Shader& shader = Shader::register_.Add(Literals::imageShader);
    static Material& material = Material::register_.Add("mat", shader, uniformsByName);

}

