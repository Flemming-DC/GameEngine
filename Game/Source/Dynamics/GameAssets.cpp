#include "GameAssets.h"
#include "Engine.h"
#include "GameLiterals.h"

Shorts;
Shader& GameAssets::shader = Shader();
Texture& GameAssets::texture = Texture();
Material& GameAssets::material = Material();


void GameAssets::Setup()
{
    if (UuidCreator::IsInitialized(material.GetID()))
        return; // if material is init, then the others are so too.

    // parameters
    map_uo<string, std::any> uniformsByName = {
        {Literals::u_textureSampler, &texture},
        {Literals::u_color, glm::vec4(0.8f, 0.3f, 0.8f, 1.0f)},
    };

    // assets
    shader = Shader::register_.Add(Literals::imageShader);
    texture = Texture::register_.Add(Literals::blizzardAttackingFans);
    material = Material::register_.Add(Literals::dummyMat, shader, uniformsByName);

}

