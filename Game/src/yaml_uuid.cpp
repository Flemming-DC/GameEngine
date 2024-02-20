/*
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "UuidCreator.h"



void DummyYAML()
{
    // Create a YAML document
    YAML::Node dataSaved;
    dataSaved["name"] = "John";
    dataSaved["age"] = 40;
    dataSaved["seq"].push_back("first element");  // node["seq"] automatically becomes a sequence
    dataSaved["seq"].push_back("second element");
    //dataSaved["mirror"] = dataSaved["seq"][0];  // this creates an alias
    YAML::Node subNode;
    subNode["subName"] = "Jo";
    subNode["subAge"] = 10;
    dataSaved["subNode"] = subNode;

    // Serialize the data to a YAML file
    std::ofstream outStream("res/data.yml");
    outStream << dataSaved;
    outStream.close();


    // Load data from a YAML file
    YAML::Node dataLoaded = YAML::LoadFile("res/yaml/data.yaml");

    if (dataLoaded["name"] && dataLoaded["age"])
    {
        std::string name = dataLoaded["name"].as<std::string>();
        int age = dataLoaded["age"].as<int>();
        auto seq = dataLoaded["seq"].as<std::vector<std::string>>();
        auto subNode1 = dataLoaded["subNode"].as<YAML::Node>();
        auto subNode2 = dataLoaded["subNode"].as<std::map<std::string, std::string>>();

        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "subName: " << subNode1["subName"] << std::endl;
        std::cout << "subAge: " << subNode1["subAge"] << std::endl;

    }
    else
    {
        std::cerr << "Failed to read data from YAML file." << std::endl;
    }

    //YAML::Node node = YAML::Load("[1, 2, 3]");
    YAML::Node doc = YAML::LoadFile("res/yaml/dummy.yaml"); // really shitty error message, if path isn't found. Fix that!!


}

Material LoadMaterial(Shader& shaderOut)
{
    YAML::Node materialAsset;
    try
    {
        materialAsset = YAML::LoadFile("res/yaml/materials/BlizMat.yaml");
    }
    catch (...)
    {
        RaiseError("Failed to load materialAsset. Check that path is correct.");
    }

    try
    {
        // this has special treatmenst for color, texture and u_MVP. aka. for EVERY uniform

        YAML::Node uniforms = materialAsset["uniformsByName"].as<YAML::Node>();
        auto colorVector = uniforms["u_color"].as<std::vector<float>>();
        glm::vec4 color = { colorVector[0], colorVector[1], colorVector[2], colorVector[3] };
        Texture texture(uniforms["u_textureSampler"].as<std::string>());
        std::map<std::string, std::any> uniformsByName = {
            {"u_textureSampler", &texture},
            {"u_color", color},
            {"u_MVP", Material::MissingUniform()}
        };
        shaderOut = Shader(materialAsset["shader"].as<std::string>());
        Material material(shaderOut, uniformsByName);

        return material;
    }
    catch (...)
    {
        RaiseError("Failed to load parse materialAsset.");
    }
}

Mesh LoadMesh()
{
    YAML::Node meshAsset;
    try
    {
        meshAsset = YAML::LoadFile("res/yaml/meshes/SquareMesh.yaml");
    }
    catch (...)
    {
        RaiseError("Failed to load assets. Check that paths are correct.");
    }

    try
    {
        YAML::Node layoutAsset = meshAsset["layout"].as<YAML::Node>();
        VertexLayout vertexLayout = {
            layoutAsset["positionDimension"].as<int>(),
            layoutAsset["textureDimension"].as<int>(),
            layoutAsset["colorDimension"].as<int>(),
            layoutAsset["textureID"].as<int>(),
        };
        auto vertices = meshAsset["vertices"].as<std::vector<float>>();
        auto indices = meshAsset["indices"].as<std::vector<unsigned int>>();
        Mesh mesh(vertices, indices, vertexLayout);

        return mesh;
    }
    catch (...)
    {
        RaiseError("Failed to load parse meshAsset.");
    }

}

Entity LoadEntity(Mesh* mesh, Material* material)
{
    // set uuid's

    YAML::Node entityAsset;
    try
    {
        entityAsset = YAML::LoadFile("res/yaml/entities/Picture1.yaml");
    }
    catch (...)
    {
        RaiseError("Failed to load assets. Check that paths are correct.");
    }

    try
    {
        // make component collection dynamic
        Entity picture1(entityAsset["name"].as<std::string>());
        picture1.AddComponent<Transform>();
        picture1.AddComponent<Renderable>()->SetByInspector(mesh, material);
        picture1.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });

        return picture1;
    }
    catch (...)
    {
        RaiseError("Failed to load parse entityAsset.");
    }
}

*/

// in run function

//DummyUUID();
//DummyYAML();
/*
Mesh mesh = LoadMesh();
Shader shader("res/shaders/Image.shader");
Material material = LoadMaterial(shader);
Entity picture1 = LoadEntity(&mesh, &material);
*/


