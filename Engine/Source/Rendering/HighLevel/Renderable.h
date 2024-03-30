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
	Material material;

	void Setup(const Material& material, const Mesh& mesh);
	void Setup(const Material& material);
	void Draw(const glm::mat4& projectionView);
	static void UnBind();
	inline Material& GetMaterial() { return material; } // not used in engine
	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	
private:
	Mesh mesh;

	void OnStart() override;
	void OnDestroy() override;
};

