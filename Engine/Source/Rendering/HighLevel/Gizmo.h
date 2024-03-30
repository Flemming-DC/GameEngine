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
	Shorts;
	static Register<Gizmo> register_;
	bool showPoints = true;
	bool loop = true;

	// transform is allowed to be null
	Gizmo(vector<vec2> position2Ds, Transform* transform, vec4 color = defaultColor, bool showPoints = true, bool loop = true);
	void Draw(const glm::mat4& projectionView);
	void UnBind();

	static uuid Add(vector<vec2> position2Ds, Transform* transform, vec4 color = defaultColor, bool showPoints = true, bool loop = true);
	static vector<Gizmo>& GetData() { return register_.GetData(); }
	static uuid MakeCircle(vec2 center, float radius, Transform& transform_, vec4 color = defaultColor);
	inline uuid GetID() const { return id; }
	string to_string() const;
	static void CleanupDeadGizmos(); // removes gizmos from destroyed transforms

private:
	Mesh mesh;
	vec4 color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Material material;
	int positionCount = 0;
	uuid id;
	optional<uuid> transformID = std::nullopt;
	inline static const vec4 defaultColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
};
