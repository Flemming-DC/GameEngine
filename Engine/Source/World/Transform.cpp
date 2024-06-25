#include "Transform.h"
#include "ListTools.h"
#include "GlmTools.h"
#include "ErrorChecker.h"
#include "EngineMode.h"
#include "GlmCheck.h"

Shorts;
#define CheckStatic \
	Deny(isStatic && EngineMode::GameIsRunning(), \
	"You cannot change spatial properties of static transform, while the game is running.");

void Transform::OnDestroy()
{
	SetParent(nullptr);
}


void Transform::SetLocalPosition(vec3 pos) 
{ 
	CheckStatic;
	CacheModel(glm::translate(model, pos - localPosition));
	localPosition = pos;
}

float Transform::LocalAngle() const { return glm::eulerAngles(localRotation).z; } // this is in radians.
void Transform::SetLocalAngle(float localAngle)
{
	CheckStatic;
	auto euler = glm::eulerAngles(localRotation);
	localRotation = quat(vec3(euler.x, euler.y, localAngle));
	CacheModel(CalcModel());
}

vec3 Transform::LocalScale() const { return localScale; }
void Transform::SetLocalScale(vec3 localScale_)
{
	CheckStatic;
	if (requireUniformScale)
		localScale_ = vec3(localScale_.x); // unclear whether this is the best way to ensure uniformity
	vec3 factor = localScale_ / localScale;
	Check_vec(factor);
	localScale *= factor;
	CacheModel(glm::scale(model, factor));
}

vec2 Transform::Forward2D()
{
	return (vec2) (Rotation() * vec3(1.0f, 0.0f, 0.0f));
}


mat4 Transform::LocalModel() const
{
	// can be optimized
	return glm::translate(mat4(1.0f), localPosition) 
		* glm::mat4_cast(localRotation)
		* glm::scale(mat4(1.0f), localScale);
}
mat4 Transform::InverseLocalModel() const
{
	return glm::scale(mat4(1.0f), 1.0f / localScale)
		* glm::mat4_cast(glm::inverse(localRotation))
		* glm::translate(mat4(1.0f), -localPosition);
}

void Transform::SmoothAngle(float targetAngle, float duration)
{
	float nextAngle = glm::SmoothAngle(Angle(), targetAngle, duration);
	SetAngle(nextAngle);
}


void Transform::IncrementAngle(float deltaAngle)
{
	CheckStatic;
	auto euler = glm::eulerAngles(localRotation);
	float newLocalAngle = euler.z + deltaAngle;
	localRotation = quat(vec3(euler.x, euler.y, newLocalAngle));
	CacheModel(CalcModel());
}

void Transform::SetPosition2D(vec2 pos) 
{ 
	CheckStatic;
	vec3 delta = glm::ToVec3((pos - Position2D()));
	localPosition += delta;
	//CacheModel(glm::translate(model, delta)); // yields weirdo error
	CacheModel(CalcModel());
}
void Transform::SetAngle(float angle)
{
	CheckStatic;
	auto euler = glm::eulerAngles(localRotation);
	float newLocalAngle = euler.z + (angle - Angle());
	localRotation = quat(vec3(euler.x, euler.y, newLocalAngle));
	CacheModel(CalcModel());
}
void Transform::SetScale2D(vec2 scale, bool allowFlip)
{
	CheckStatic;
	if (!allowFlip)
		scale = glm::max(scale, vec2(0.00001f));
	if (requireUniformScale)
		scale.y = scale.x; // unclear whether this is the best way to ensure uniformity
	vec3 factor = glm::ToVec3((scale / Scale2D()), 1.0f);
	Check_vec(factor);
	localScale *= factor;
	CacheModel(glm::scale(model, factor));
}


void Transform::SetPosition(vec3 pos) 
{ 
	CheckStatic;
	vec3 delta = pos - Position();
	CacheModel(glm::translate(model, delta));
	localPosition += delta;
}
void Transform::SetRotation(quat rot) 
{ 
	CheckStatic; 
	localRotation *= (rot * glm::inverse(Rotation()));
	CacheModel(CalcModel());
}
void Transform::SetScale(vec3 scale) 
{
	CheckStatic;
	if (requireUniformScale)
		scale = vec3(scale.x); // unclear whether this is the best way to ensure uniformity
	vec3 factor = scale / Scale();
	Check_vec(factor);
	localScale *= factor;
	CacheModel(glm::scale(model, factor));
}

vec3 Transform::Scale() const
{
	return vec3(
		glm::Magnitude(model[0]),
		glm::Magnitude(model[1]),
		glm::Magnitude(model[2])
	);
}
mat4 Transform::CalcModel() const
{
	if (!HasParent())
		return LocalModel();
	else
		return Parent().Model() * LocalModel();
}
mat4 Transform::InverseModel() const
{	
	if (!HasParent())
		return InverseLocalModel();
	else
		return glm::inverse(model);;
}


Transform* Transform::TryParent() const
{
	return Entity::TryGetComponent<Transform>(parentID);
}

void Transform::SetParent(Transform* newParent) 
{
	Deny(isStatic && newParent && !newParent->IsStatic(),
		"You cannot set a static transform to be the child of a non-static parent.");
	assert(newParent != this);
	assert(!newParent || !newParent->IsDescendantOf(*this));

	// the local data (which is stored explicitly) is relative to a given parent.
	// therefore we must adjust for the effect that changing parent has on the world
	// position, rotation and scale
	Transform* oldParent = TryParent();
	mat4 oldParentModel = HasParent() ? oldParent->Model() : mat4(1.0f);
	mat4 newParentInverseModel = newParent ? newParent->InverseModel() : mat4(1.0f);
	SetLocalDataUsingMatrix(newParentInverseModel * oldParentModel * LocalModel()); // oldParentModel * LocalModel() = CalcModel()

	if (oldParent)
		Tools::Remove(oldParent->children, this);
	parentID = newParent ? newParent->GetID() : uuid();
	if (newParent)
		newParent->children.push_back(this);

	InDebug(if (newParent)
	{
		for (const auto& newSibling : newParent->children)
		{
			Assert(newSibling, "newSibling is null: " + to_string());
			Assert(newSibling->TryParent() == newParent, "newParent must be my newSiblings parent.");
		}
	});
}

vector<Transform*> Transform::GetChildren() const
{
	InDebug(
		for (const auto& child : children)
		{
			Assert(child, "child is null: " + to_string());
			Assert(child->TryParent() == this, "I must be my childrens parent.");
		}
	);

	return children;
}

string Transform::GetPath() const
{
	if (!HasParent())
		return entity().Name();
	else
		return Parent().GetPath() + "/" + entity().Name();
}

bool Transform::IsDescendantOf(const Transform& putativeAncestor) const
{
	auto parent = TryParent();
	if (parent == nullptr)
		return false;
	else if (*parent == putativeAncestor)
		return true;
	else
		return parent->IsDescendantOf(putativeAncestor);
}

Transform& Transform::Root()
{
	Transform* ancestor = this;
	while (ancestor->HasParent())
		ancestor = &ancestor->Parent();
	return *ancestor;
}

// isPosition is used to distinguish position vector from other vectors (e.g. a velocity vector)
vec3 Transform::ToWorldSpace(vec3 localVector, bool isPosition) const
{
	// we extend the localPosition using a homogenous coordinate
	mat4 matrix = isPosition ? model : glm::mat4_cast(Rotation());
	return matrix * vec4(localVector.x, localVector.y, localVector.z, 1);
}
vec2 Transform::ToWorldSpace(vec2 localVector2D, bool isPosition) const
{
	mat4 matrix = isPosition ? model : glm::mat4_cast(Rotation());
	return matrix * vec4(localVector2D.x, localVector2D.y, 0, 1);
}
vec3 Transform::ToLocalSpace(vec3 worldVector, bool isPosition) const
{
	mat4 matrix = isPosition ? InverseModel() : glm::mat4_cast(glm::inverse(Rotation()));
	return matrix * vec4(worldVector.x, worldVector.y, worldVector.z, 1);
}
vec2 Transform::ToLocalSpace(vec2 worldVector2D, bool isPosition) const
{
	mat4 matrix = isPosition ? InverseModel() : glm::mat4_cast(glm::inverse(Rotation()));
	return matrix * vec4(worldVector2D.x, worldVector2D.y, 0, 1);
}

void Transform::SetLocalDataUsingMatrix(const mat4& transformMatrix)
{
	// We omit CheckStatic, since this function is called by setParent, 
	// which is called during reload scene, and because it is private.
	localPosition = vec3(transformMatrix[3]);

	localScale.x = glm::length(vec3(transformMatrix[0]));
	localScale.y = glm::length(vec3(transformMatrix[1]));
	localScale.z = glm::length(vec3(transformMatrix[2]));

	glm::mat3 rotationMatrix(transformMatrix);
	localRotation = glm::quat_cast(rotationMatrix);
	CacheModel(CalcModel());
}

void Transform::SetStatic(bool becomeStatic)
{
	Deny(EngineMode::GameIsRunning(), 
		"cannot set transform to static/non-static, while the game is running");
	isStatic = becomeStatic;
}


void Transform::CacheModel(const mat4& m)
{
	model = m;
	for (Transform* child : children)
		child->CacheModel(m * child->LocalModel());
}



void Transform::Save(YAML::Node& node) const
{
	node["localPosition"] = localPosition;
	node["localRotation"] = localRotation;
	node["localScale"] = localScale;
	if (UuidCreator::IsInitialized(parentID))
		node["parent"] = parentID;
	node["isStatic"] = isStatic;
}

void Transform::Load(const YAML::Node& node)
{
	if (node["parent"])
	{
		auto parentID = node["parent"].as<uuid>();
		Assert(Entity::ExistsComponent(parentID), "parent not found. parentID = ", parentID);
		SetParent(&Entity::GetComponent<Transform>(parentID));
	}
	localPosition = node["localPosition"].as<vec3>();
	localRotation = node["localRotation"].as<quat>();
	localScale = node["localScale"].as<vec3>();
	isStatic = node["isStatic"].as<bool>(); // we dont use SetStatic, since the check doesnt count
	CacheModel(CalcModel());
}

