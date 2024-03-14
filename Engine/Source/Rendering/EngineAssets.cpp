#include "EngineAssets.h"
#include "UuidCreator.h"


Shader& EngineAssets::solidColorShader = Shader();
Material& EngineAssets::gizmoMaterial = Material();
Mesh& EngineAssets::squareMesh = Mesh();

void EngineAssets::Setup()
{
	solidColorShader = Shader::register_.Add("res/shaders/SolidColor.shader");

	std::map<std::string, std::any> uniformsByName = { {"u_color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)} };
	gizmoMaterial = Material::register_.Add(EngineAssets::SolidColorShader(), uniformsByName);


    std::vector<float> vertexVector =
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
    squareMesh = Mesh::register_.Add(vertexVector, indexVector, VertexLayout());

}












