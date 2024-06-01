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
	static std::vector<uuids::uuid> allRenderables;

	void Draw(const glm::mat4& projectionView);
	static void UnBind();

	inline void SetMaterial(const Material& material_) { material = material_; } // copy, not ref, not used in engine
	inline void SetMesh(const Mesh& mesh_) { mesh = mesh_; } // copy, not ref, not used in engine
	inline Material& GetMaterial() { return material; } // not used in engine
	inline Mesh& GetMesh() { return mesh; } // not used in engine
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	int InitOrder() override { return -1200; }
	inline int DrawOrder() const { return drawOrder; }
	inline void SetDrawOrder(int drawOrder_) { drawOrder = drawOrder_; }
	
private:
	Mesh mesh;
	Material material;
	int drawOrder = 0;

	void OnStart() override;
	void OnDestroy() override;
};

