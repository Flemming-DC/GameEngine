#include "EngineAssets.h"
#include "UuidCreator.h"
#include "EngineFiles.h"

Shorts;
Shader& EngineAssets::solidColorShader = Shader();
Material& EngineAssets::defaultMaterial = Material();
Mesh& EngineAssets::squareMesh = Mesh();

Shader& EngineAssets::imageShader = Shader();
Texture& EngineAssets::defualtImage = Texture();
Material& EngineAssets::imageMaterial = Material();

void EngineAssets::Setup()
{
    solidColorShader = Shader::register_.Add(EngineFiles::colorShader);
    map_uo<string, std::any> uniformsByName = { {"u_color", vec4(0.5f, 0.5f, 0.5f, 1.0f)} };
    defaultMaterial = Material::register_.Add("default", EngineAssets::SolidColorShader(), uniformsByName);


    map_uo<string, std::any> ImageUniforms = {
        {"u_textureSampler", &defualtImage},
        {"u_color", glm::vec4(1.0f)},
    };
    imageShader = Shader::register_.Add(EngineFiles::imageShader);
    defualtImage = Texture::register_.Add(EngineFiles::defaultImage);
    imageMaterial = Material::register_.Add("image", imageShader, ImageUniforms);

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
    squareMesh = Mesh::register_.Add("squareMesh", vertexVector, indexVector, VertexLayout());

}












