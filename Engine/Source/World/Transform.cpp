#include "Transform.h"
#include "ListTools.h"
#include "GlmTools.h"
#include "ErrorChecker.h"
#include <windows.h>



void Transform::OnDestroy()
{
	SetParent(nullptr);
}
glm::mat4 Transform::GetLocalModel() const
{
	return glm::translate(glm::mat4(1.0f), localPosition)
		* glm::mat4_cast(localRotation)
		* glm::scale(glm::mat4(1.0f), localScale);
}
glm::mat4 Transform::GetInverseLocalModel() const
{
	return glm::scale(glm::mat4(1.0f), 1.0f / localScale)
		* glm::mat4_cast(glm::inverse(localRotation))
		* glm::translate(glm::mat4(1.0f), -localPosition);
}

glm::vec3 Transform::GetPosition() const
{
	if (GetParent() == nullptr)
		return localPosition;
	else
		return parent->GetPosition() + parent->GetRotation() * localPosition; // this handling of rotation is inefficient
}
glm::quat Transform::GetRotation() const
{
	if (GetParent() == nullptr)
		return localRotation;
	else
		return parent->GetRotation() * localRotation;
}
glm::vec3 Transform::GetScale() const
{
	if (GetParent() == nullptr)
		return localScale;
	else
		return parent->GetScale() * localScale;
}
glm::mat4 Transform::GetModel() const
{
	if (GetParent() == nullptr)
		return GetLocalModel();
	else
		return parent->GetModel() * GetLocalModel(); // using this by the renderer is inefficient
}
glm::mat4 Transform::GetInverseModel() const
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
	glm::mat4 oldParentModel = GetParent() ? parent->GetModel() : glm::mat4(1.0f);
	glm::mat4 newParentInverseModel = newParent ? newParent->GetInverseModel() : glm::mat4(1.0f);
	SetLocalDataUsingTransform(newParentInverseModel * oldParentModel * GetModel());

	if (parent)
		Tools::Remove(parent->children, this);
	parent = newParent;
	if (newParent)
		newParent->children.push_back(this);
}
std::vector<Transform*> Transform::GetChildren() const
{
	return children;
}
std::string Transform::GetPath() const
{
	if (GetParent() == nullptr)
		return GetEntity().GetName();
	else
		return parent->GetPath() + "/" + GetEntity().GetName();
}

glm::vec3 Transform::ToWorldSpace(glm::vec3 localPosition_, bool isPosition) const
{
	// we extend the localPosition using a homogenous coordinate
	glm::mat4 matrix = isPosition ? GetModel() : glm::mat4_cast(GetRotation());
	return matrix * glm::vec4(localPosition_.x, localPosition_.y, localPosition_.z, 1);
}
glm::vec2 Transform::ToWorldSpace(glm::vec2 localPosition2D_, bool isPosition) const
{
	glm::mat4 matrix = isPosition ? GetModel() : glm::mat4_cast(GetRotation());
	return matrix * glm::vec4(localPosition2D_.x, localPosition2D_.y, 0, 1);
}
glm::vec3 Transform::ToLocalSpace(glm::vec3 worldPosition, bool isPosition) const
{
	glm::mat4 matrix = isPosition ? GetInverseModel() : glm::mat4_cast(glm::inverse(GetRotation()));
	return matrix * glm::vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1);
}
glm::vec2 Transform::ToLocalSpace(glm::vec2 worldPosition2D, bool isPosition) const
{
	glm::mat4 matrix = isPosition ? GetInverseModel() : glm::mat4_cast(glm::inverse(GetRotation()));
	return matrix * glm::vec4(worldPosition2D.x, worldPosition2D.y, 0, 1);
}

void Transform::SetLocalDataUsingTransform(const glm::mat4& transform)
{
	localPosition = glm::vec3(transform[3]);

	localScale.x = glm::length(glm::vec3(transform[0]));
	localScale.y = glm::length(glm::vec3(transform[1]));
	localScale.z = glm::length(glm::vec3(transform[2]));

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
	localPosition = node["localPosition"].as<glm::vec3>();
	localRotation = node["localRotation"].as<glm::quat>();
	localScale = node["localScale"].as<glm::vec3>();
	if (node["parent"])
	{
		auto parentID = node["parent"].as<uuids::uuid>();
		SetParent(&Entity::GetComponent<Transform>(parentID));
	}
}

