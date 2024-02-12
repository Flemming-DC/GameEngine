#include "Scene.h"


Scene::Scene(std::string name) : name(name)
{
	entities = MakeEntities();
	LoadSpatialData(entities);
}


void Scene::LoadSpatialData(std::vector<Entity> entities)
{

}

