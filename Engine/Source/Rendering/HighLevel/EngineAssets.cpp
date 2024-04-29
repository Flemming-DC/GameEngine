#include "EngineAssets.h"
#include "UuidCreator.h"
#include "EngineLiterals.h"

Shorts;
Shader& EngineAssets::solidColorShader = Shader();
Material& EngineAssets::defaultMaterial = Material();
Mesh& EngineAssets::squareMesh = Mesh();

Shader& EngineAssets::imageShader = Shader();
Texture& EngineAssets::defualtImage = Texture();
Material& EngineAssets::imageMaterial = Material();

void EngineAssets::Setup()
{
    solidColorShader = Shader::register_.Add(Literals::colorShader);
    map_uo<string, std::any> uniformsByName = { {Literals::u_color, vec4(0.5f, 0.5f, 0.5f, 1.0f)} };
    defaultMaterial = Material::register_.Add(Literals::colorMaterialName, solidColorShader, uniformsByName);


    map_uo<string, std::any> ImageUniforms = {
        {Literals::u_textureSampler, &defualtImage},
        {Literals::u_color, glm::vec4(1.0f)},
    };
    imageShader = Shader::register_.Add(Literals::imageShader);
    defualtImage = Texture::register_.Add(Literals::defaultImage);
    imageMaterial = Material::register_.Add(Literals::imageMaterialName, imageShader, ImageUniforms);

    vector<float> vertexVector =
    {//  pos.x  pos.y tex.u tex.v
        -0.5f, -0.5f, 0.0f, 0.0f, // LD
        -0.5f,  0.5f, 0.0f, 1.0f, // LU
         0.5f,  0.5f, 1.0f, 1.0f, // RU
         0.5f, -0.5f, 1.0f, 0.0f, // RD
    };
    std::vector<unsigned int> indexVector =
    {
        0, 3, 2, // triangle(LD, RD, RU)
        0, 1, 2, // triangle(LD, LU, RU)
    };
    squareMesh = Mesh::register_.Add(Literals::squareMeshName, vertexVector, indexVector, VertexLayout());

}












