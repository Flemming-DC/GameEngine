#include "Transform.h"
#include "Tools\ListTools.h"
#include "GlmTools.h"



void Transform::OnDestroyed()
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
	if (parent == nullptr)
		return localPosition;
	else
		return parent->GetPosition() + parent->GetRotation() * localPosition; // this handling of rotation is inefficient
}
glm::quat Transform::GetRotation() const
{
	if (parent == nullptr)
		return localRotation;
	else
		return parent->GetRotation() * localRotation;
}
glm::vec3 Transform::GetScale() const
{
	if (parent == nullptr)
		return localScale;
	else
		return parent->GetScale() * localScale;
}
glm::mat4 Transform::GetModel() const
{
	if (parent == nullptr)
		return GetLocalModel();
	else
		return parent->GetModel() * GetLocalModel(); // using this by the renderer is inefficient
}
glm::mat4 Transform::GetInverseModel() const
{
	if (parent == nullptr)
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
	if (parent)
		Tools::Remove(parent->children, this);
	// apply parent transform to this, in order to counteract the loss of its transform
	parent = newParent;
	// apply inverse newParent transform to this, in order to counteract the gain of its transform
	if (newParent)
		newParent->children.push_back(this);;

}
std::vector<Transform*> Transform::GetChildren() const
{
	return children;
}
std::string Transform::GetPath() const
{
	if (parent == nullptr)
		return GetEntity().name;
	else
		return parent->GetPath() + "/" + GetEntity().name;
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





