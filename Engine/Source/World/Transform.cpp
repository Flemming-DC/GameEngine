#include "Transform.h"
#include "ListTools.h"
#include "GlmTools.h"
#include "ErrorChecker.h"
#include <windows.h>

Shorts;

void Transform::OnDestroy()
{
	SetParent(nullptr);
}

vec3 Transform::GetLocalScale() const { return localScale; }
void Transform::SetLocalScale(vec3 localScale_)
{
	if (requireUniformScale)
		localScale_ = vec3(localScale_.x); // unclear whether this is the best way to ensure uniformity
	localScale = localScale_;
}


mat4 Transform::GetLocalModel() const
{
	return glm::translate(mat4(1.0f), localPosition)
		* glm::mat4_cast(localRotation)
		* glm::scale(mat4(1.0f), localScale);
}
mat4 Transform::GetInverseLocalModel() const
{
	return glm::scale(mat4(1.0f), 1.0f / localScale)
		* glm::mat4_cast(glm::inverse(localRotation))
		* glm::translate(mat4(1.0f), -localPosition);
}

void Transform::IncrementLocalAngle(float deltaAngle)
{
	float localAngle = glm::eulerAngles(localRotation).z + deltaAngle;
	localRotation = quat(vec3(0.0f, 0.0f, localAngle));
}

void Transform::SetPosition2D(vec2 pos) { localPosition += glm::ToVec3((pos - Position2D())); }
void Transform::SetAngle(float angle)
{
	float oldLocalAngle = glm::eulerAngles(localRotation).z;
	float newLocalAngle = oldLocalAngle + (angle - Angle());
	localRotation = quat(vec3(0.0f, 0.0f, newLocalAngle));
}
void Transform::SetScale2D(vec2 scale) 
{
	if (requireUniformScale)
		scale.y = scale.x; // unclear whether this is the best way to ensure uniformity
	localScale *= glm::ToVec3((scale / Scale2D())); 
}

vec2 Transform::Position2D() const { return (vec2)GetPosition(); }
float Transform::Angle() const { return  glm::eulerAngles(GetRotation()).z; } // this is in radians.
vec2 Transform::Scale2D() const { return (vec2)GetScale(); }

void Transform::SetPosition(vec3 pos) { localPosition += (pos - GetPosition()); }
void Transform::SetRotation(quat rot) { localRotation *= (rot * glm::inverse(GetRotation())); }
void Transform::SetScale(vec3 scale) 
{
	if (requireUniformScale)
		scale = vec3(scale.x); // unclear whether this is the best way to ensure uniformity
	localScale *= (scale / GetScale());
}

vec3 Transform::GetPosition() const
{
	if (GetParent() == nullptr)
		return localPosition;
	else
		return parent->GetPosition() + parent->GetRotation() * localPosition; // this handling of rotation is inefficient
}
quat Transform::GetRotation() const
{
	if (GetParent() == nullptr)
		return localRotation;
	else
		return parent->GetRotation() * localRotation;
}
vec3 Transform::GetScale() const
{
	if (GetParent() == nullptr)
		return localScale;
	else
		return parent->GetScale() * localScale;
}
mat4 Transform::GetModel() const
{
	if (GetParent() == nullptr)
		return GetLocalModel();
	else
		return parent->GetModel() * GetLocalModel(); // using this by the renderer is inefficient
}
mat4 Transform::GetInverseModel() const
{
	if (GetParent() == nullptr)
		return GetInverseLocalModel();
	else
		return GetInverseLocalModel() * parent->GetInverseModel();
}


Transform* Transform::GetParent() const
{
	return parent;
}
void Transform::SetParent(Transform* newParent) 
{
	// the local data (which is stored explicitly) is relative to a given parent.
	// therefore we must adjust for the effect that changing parent has on the world
	// position, rotation and scale
	mat4 oldParentModel = GetParent() ? parent->GetModel() : mat4(1.0f);
	mat4 newParentInverseModel = newParent ? newParent->GetInverseModel() : mat4(1.0f);
	SetLocalDataUsingTransform(newParentInverseModel * oldParentModel * GetModel());

	if (parent)
		Tools::Remove(parent->children, this);
	parent = newParent;
	if (newParent)
		newParent->children.push_back(this);
}
vector<Transform*> Transform::GetChildren() const
{
	#ifdef _DEBUG
	for (const auto& child : children)
		if (!child)
			RaiseError("child is null: ", to_string());
	#endif // _DEBUG

	return children;
}
string Transform::GetPath() const
{
	if (GetParent() == nullptr)
		return GetEntity().GetName();
	else
		return parent->GetPath() + "/" + GetEntity().GetName();
}

bool Transform::IsDescendantOf(const Transform& putativeAncestor) const
{
	auto parent = GetParent();
	if (parent == nullptr)
		return false;
	else if (*parent == putativeAncestor)
		return true;
	else
		return parent->IsDescendantOf(putativeAncestor);
}

// isPosition is used to distinguish position vector from other vectors (e.g. a velocity vector)
vec3 Transform::ToWorldSpace(vec3 localVector, bool isPosition) const
{
	// we extend the localPosition using a homogenous coordinate
	mat4 matrix = isPosition ? GetModel() : glm::mat4_cast(GetRotation());
	return matrix * vec4(localVector.x, localVector.y, localVector.z, 1);
}
vec2 Transform::ToWorldSpace(vec2 localVector2D, bool isPosition) const
{
	mat4 matrix = isPosition ? GetModel() : glm::mat4_cast(GetRotation());
	return matrix * vec4(localVector2D.x, localVector2D.y, 0, 1);
}
vec3 Transform::ToLocalSpace(vec3 worldVector, bool isPosition) const
{
	mat4 matrix = isPosition ? GetInverseModel() : glm::mat4_cast(glm::inverse(GetRotation()));
	return matrix * vec4(worldVector.x, worldVector.y, worldVector.z, 1);
}
vec2 Transform::ToLocalSpace(vec2 worldVector2D, bool isPosition) const
{
	mat4 matrix = isPosition ? GetInverseModel() : glm::mat4_cast(glm::inverse(GetRotation()));
	return matrix * vec4(worldVector2D.x, worldVector2D.y, 0, 1);
}

void Transform::SetLocalDataUsingTransform(const mat4& transform)
{
	localPosition = vec3(transform[3]);

	localScale.x = glm::length(vec3(transform[0]));
	localScale.y = glm::length(vec3(transform[1]));
	localScale.z = glm::length(vec3(transform[2]));

	glm::mat3 rotationMatrix(transform);
	localRotation = glm::quat_cast(rotationMatrix);
}



void Transform::Save(YAML::Node& node) const
{
	node["localPosition"] = localPosition;
	node["localRotation"] = localRotation;
	node["localScale"] = localScale;
	if (parent != nullptr)
		node["parent"] = parent->GetID();
}

void Transform::Load(const YAML::Node& node)
{
	if (node["parent"])
	{
		auto parentID = node["parent"].as<uuid>();
		SetParent(&Entity::GetComponent<Transform>(parentID));
	}
	localPosition = node["localPosition"].as<vec3>();
	localRotation = node["localRotation"].as<quat>();
	localScale = node["localScale"].as<vec3>();
}

