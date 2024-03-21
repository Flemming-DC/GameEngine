#pragma once
#include <vector>
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"
#include "glm/glm.hpp"
#include "Transform.h"
#include "Register.h"
#include <stduuid/uuid.h>
#include <optional> 


// you should only create or draw gizmo if in editor
// 
// this class has a lot of code dublication from the Renderable class, since a Gizmo is also conceptually
// a renderable. However, Renderable inhirits from component and Gizmo does not, which forces the dublication
class Gizmo
{
public:
	static Register<Gizmo> register_;
	bool showPoints = true;
	bool loop = true;

	// transform is allowed to be null
	Gizmo(std::vector<glm::vec2> position2Ds, Transform* transform, 
		glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), bool showPoints = true, bool loop = true);
	void Draw();
	void UnBind();

	static uuids::uuid MakeCircle(
		glm::vec2 center, 
		float radius, 
		Transform& transform_, 
		glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	inline uuids::uuid GetID() const { return id; }
	std::string to_string() const;
	static void CleanupDeadGizmos(); // removes gizmos from destroyed transforms

private:
	Mesh mesh;
	glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Material material;
	Transform* transform = nullptr;
	int positionCount = 0;
	uuids::uuid id;
	bool attachedToTransform = false; // equal to transform != nullptr at construction
	std::optional<uuids::uuid> transformID = std::nullopt;
};
