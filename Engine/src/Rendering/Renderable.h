#pragma once
#include <vector>
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Component.h"


// evt. use batch-rendering
class Renderable : public Component
{
public:
	static std::vector<Renderable*> allRenderables;

	void SetByInspector(Mesh* mesh, Material* material);
	void Draw();
	static void UnBind();

private:
	Mesh* mesh; // show in inspector
	Material* material; // show in inspector

	void OnConstructed() override;
	void OnDestroyed() override;
};

