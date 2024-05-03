#include "RenderSaver.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

void RenderSaver::Save()
{
	Texture::naming.Save("textureNames.yml");
	Material::naming.Save("materialNames.yml");
	Mesh::naming.Save("meshNames.yml");
}

void RenderSaver::Load()
{
	Texture::naming.Load("textureNames.yml");
	Material::naming.Load("materialNames.yml");
	Mesh::naming.Load("meshNames.yml");
}
