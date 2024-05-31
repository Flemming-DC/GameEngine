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

float Transform::LocalAngle() const { return  glm::eulerAngles(localRotation).z; } // this is in radians.
void Transform::SetLocalAngle(float localAngle)
{
	auto euler = glm::eulerAngles(localRotation);
	localRotation = quat(vec3(euler.x, euler.y, localAngle));
}

vec3 Transform::LocalScale() const { return localScale; }
void Transform::SetLocalScale(vec3 localScale_)
{
	if (requireUniformScale)
		localScale_ = vec3(localScale_.x); // unclear whether this is the best way to ensure uniformity
	localScale = localScale_;
}

vec2 Transform::Forward2D()
{
	return (vec2) (GetRotation() * vec3(1.0f, 0.0f, 0.0f));
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

void Transform::SmoothAngle(float targetAngle, float step)
{
	float nextAngle = glm::SmoothAngle(Angle(), targetAngle, step);
	SetAngle(nextAngle);
}


void Transform::IncrementAngle(float deltaAngle)
{
	auto euler = glm::eulerAngles(localRotation);
	float newLocalAngle = euler.z + deltaAngle;
	localRotation = quat(vec3(euler.x, euler.y, newLocalAngle));
}

void Transform::SetPosition2D(vec2 pos) { localPosition += glm::ToVec3((pos - Position2D())); }
void Transform::SetAngle(float angle)
{
	auto euler = glm::eulerAngles(localRotation);
	float newLocalAngle = euler.z + (angle - Angle());
	localRotation = quat(vec3(euler.x, euler.y, newLocalAngle));
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
	if (Parent() == nullptr)
		return localPosition;
	else
		return Parent()->GetPosition() + Parent()->GetRotation() * localPosition; // this handling of rotation is inefficient
}
quat Transform::GetRotation() const
{
	if (Parent() == nullptr)
		return localRotation;
	else
		return Parent()->GetRotation() * localRotation;
}
vec3 Transform::GetScale() const
{
	if (Parent() == nullptr)
		return localScale;
	else
		return Parent()->GetScale() * localScale;
}
mat4 Transform::GetModel() const
{
	if (Parent() == nullptr)
		return GetLocalModel();
	else
		return Parent()->GetModel() * GetLocalModel(); // using this by the renderer is inefficient
}
mat4 Transform::GetInverseModel() const
{
	if (Parent() == nullptr)
		return GetInverseLocalModel();
	else
		return GetInverseLocalModel() * Parent()->GetInverseModel();
}


Transform* Transform::Parent() const
{
	return Entity::TryGetComponent<Transform>(parentID);
}

void Transform::SetParent(Transform* newParent) 
{
	assert(newParent != this);
	assert(!newParent || !newParent->IsDescendantOf(*this));

	// the local data (which is stored explicitly) is relative to a given parent.
	// therefore we must adjust for the effect that changing parent has on the world
	// position, rotation and scale
	Transform* oldParent = Parent();
	mat4 oldParentModel = Parent() ? oldParent->GetModel() : mat4(1.0f);
	mat4 newParentInverseModel = newParent ? newParent->GetInverseModel() : mat4(1.0f);
	SetLocalDataUsingMatrix(newParentInverseModel * oldParentModel * GetLocalModel()); // oldParentModel * GetLocalModel() = GetModel()

	if (oldParent)
		Tools::Remove(oldParent->children, this);
	parentID = newParent ? newParent->GetID() : uuid();
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
	if (Parent() == nullptr)
		return entity().Name();
	else
		return Parent()->GetPath() + "/" + entity().Name();
}

bool Transform::IsDescendantOf(const Transform& putativeAncestor) const
{
	auto parent = Parent();
	if (parent == nullptr)
		return false;
	else if (*parent == putativeAncestor)
		return true;
	else
		return parent->IsDescendantOf(putativeAncestor);
}

bool Transform::IsTransformFullyEnabled() const
{
	auto parent = Parent();
	return parent ? Enabled() && parent->IsTransformFullyEnabled() : Enabled();
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

void Transform::SetLocalDataUsingMatrix(const mat4& transformMatrix)
{
	localPosition = vec3(transformMatrix[3]);

	localScale.x = glm::length(vec3(transformMatrix[0]));
	localScale.y = glm::length(vec3(transformMatrix[1]));
	localScale.z = glm::length(vec3(transformMatrix[2]));

	glm::mat3 rotationMatrix(transformMatrix);
	localRotation = glm::quat_cast(rotationMatrix);
}



void Transform::Save(YAML::Node& node) const
{
	node["localPosition"] = localPosition;
	node["localRotation"] = localRotation;
	node["localScale"] = localScale;
	if (UuidCreator::IsInitialized(parentID))
		node["parent"] = parentID;
}

void Transform::Load(const YAML::Node& node)
{
	if (node["parent"])
	{
		auto parentID = node["parent"].as<uuid>();
		if (!Entity::ExistsComponent(parentID))
			RaiseError("parent not found. parentID = ", parentID);
		SetParent(&Entity::GetComponent<Transform>(parentID));
	}
	localPosition = node["localPosition"].as<vec3>();
	localRotation = node["localRotation"].as<quat>();
	localScale = node["localScale"].as<vec3>();
}

