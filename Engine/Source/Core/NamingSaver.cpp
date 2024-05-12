#include "NamingSaver.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "StoredEntity.h"
#include "Scene.h"


void NamingSaver::Save()
{
	Texture::naming.Save("textureNames.yml");
	Material::naming.Save("materialNames.yml");
	Mesh::naming.Save("meshNames.yml");
	StoredEntity::naming.Save("entityNames.yml");
	Scene::naming.Save("sceneNames.yml");
	
}

void NamingSaver::Load()
{
	Texture::naming.Load("textureNames.yml");
	Material::naming.Load("materialNames.yml");
	Mesh::naming.Load("meshNames.yml");
	StoredEntity::naming.Load("entityNames.yml");
	Scene::naming.Load("sceneNames.yml");

}
