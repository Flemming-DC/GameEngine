#include "Scene.h"
#include "ErrorChecker.h"
#define YAML_CPP_STATIC_DEFINE // put evt. denne macro i en dedikeret Asset class eller YML class
#include <yaml-cpp/yaml.h>
#include <fstream>

void Scene::Setup(std::string name_) 
{
	name = name_;
	entityIDs = MakeEntities();
	//Load();
}

void Scene::Load()
{

    // really shitty error message, if path isn't found. Fix that!!
    YAML::Node dataLoaded = YAML::LoadFile("res/data.yml");

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


}


void Scene::Save()
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
    std::ofstream outStream("res/data.yml"); // insert name
    outStream << dataSaved;
    outStream.close();




}



