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
	static std::vector<Gizmo*> allGizmos;
	static std::map<uuids::uuid, Gizmo&> allGizmosMap;
	static Register<Gizmo> register_;
	bool showPoints = true;
	bool loop = true;

	Gizmo();
	Gizmo(std::vector<glm::vec2> position2Ds, Transform* transform, glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		//{ Setup(position2Ds, transform, color); } // transform is allowed to be null
	void Setup(std::vector<glm::vec2> position2Ds, Transform* transform, glm::vec4 color);
	~Gizmo();
	void Draw();
	void UnBind();

	static uuids::uuid MakeCircle(
		glm::vec2 center, 
		float radius, 
		Transform& transform_, 
		glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	inline uuids::uuid GetID() const { return id; }

private:
	Mesh mesh;
	glm::vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Material material;
	Transform* transform = nullptr;
	int positionCount = 0;
	bool initialized = false;
	uuids::uuid id;
};
