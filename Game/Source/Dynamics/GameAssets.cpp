#include "GameAssets.h"
#include "Engine.h"
#include "GameFiles.h"

Shorts;
Shader& GameAssets::shader = Shader();
Texture& GameAssets::texture = Texture();
Material& GameAssets::material = Material();


void GameAssets::Setup()
{
    // parameters
    map_uo<string, std::any> uniformsByName = {
        {"u_textureSampler", &texture},
        {"u_color", glm::vec4(0.8f, 0.3f, 0.8f, 1.0f)},
    };

    // assets
    shader = Shader::register_.Add(EngineFiles::imageShader);
    texture = Texture::register_.Add(GameFiles::blizzardAttackingFans);
    material = Material::register_.Add("mat", shader, uniformsByName);

}

