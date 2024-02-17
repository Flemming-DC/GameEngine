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

	void SetByInspector(const Mesh& mesh, const Material& material);
	void Draw();
	static void UnBind();
	Material& GetMaterial() { return material; } // not used in engine
	
private:
	Mesh mesh;
	Material material;

	void OnConstructed() override;
	void OnDestroyed() override;
};

