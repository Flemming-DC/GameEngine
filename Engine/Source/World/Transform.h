#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include "Component.h"
#include "YmlTools.h"

// evt. use local model as the authoritative parameter with model cached and everything else generated from those two.
// evt. apply to caching in self and ancestors. use set local / glocal parameters instead of direct exposure of local
// parameters to control updates of the caching
// evt. use ToWorldSpace to calculate world position, rotation and scale
// evt. enforce that localScale cannot be zero or negative
// toggle enforce proportions and use this by the circle collider
class Transform : public Component
{
	Shorts;
public:
	vec3 localPosition = vec3(0.0f); 
	quat localRotation = glm::NoRotation();
	bool requireUniformScale = true;

	float LocalAngle() const;
	void SetLocalAngle(float localAngle);
	vec3 LocalScale() const;
	void SetLocalScale(vec3 localScale);
	vec2 Forward2D();

	void SmoothAngle(float targetAngle, float duration);
	void IncrementAngle(float deltaAngle);
	void IncrementPosition2D(vec2 deltaPos) { SetPosition2D(Position2D() + deltaPos); };
	void SetPosition2D(vec2 pos);
	void SetAngle(float angle);
	void SetScale2D(vec2 scale);
	vec2 Position2D() const;
	float Angle() const;
	vec2 Scale2D() const;

	void SetPosition(vec3 pos);
	void SetRotation(quat rot);
	void SetScale(vec3 scale);
	vec3 GetPosition() const;
	quat GetRotation() const;
	vec3 GetScale() const;
	mat4 GetModel() const;
	mat4 GetInverseModel() const; // probably only used by camera
	void SetParent(Transform* newParent);
	vector<Transform*> GetChildren() const;
	string GetPath() const; // could also be called to_string
	bool IsDescendantOf(const Transform& putativeAncestor) const;
	inline bool IsTransformFullyEnabled() const { return Enabled() && (!HasParent() || Parent().IsTransformFullyEnabled()); }
	Transform& Root();
	inline bool HasParent() const { return UuidCreator::IsInitialized(parentID); }
	inline bool IsStatic() const { return isStatic; }
	void SetStatic(bool becomeStatic);

	vec3 ToWorldSpace(vec3 position, bool isPosition) const;
	vec2 ToWorldSpace(vec2 position2D, bool isPosition) const;
	vec3 ToLocalSpace(vec3 position, bool isPosition) const;
	vec2 ToLocalSpace(vec2 position2D, bool isPosition) const;

	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	int InitOrder() override { return -2000; }

private:
	uuid parentID;
	bool isUpToDate = false; // i.e. caches are up to data
	//glm::mat4 model = glm::mat4(1.0f); // cached value
	vector<Transform*> children;
	vec3 localScale = vec3(1.0f);
	bool isStatic = false;

	void OnStart() override { unique = true; }
	void OnDestroy() override;
	mat4 GetLocalModel() const;
	mat4 GetInverseLocalModel() const; // probably only used by camera
	void SetLocalDataUsingMatrix(const mat4& transform);
	Transform* TryParent() const;
	inline Transform& Parent() const { return Entity::GetComponent<Transform>(parentID); }

};


