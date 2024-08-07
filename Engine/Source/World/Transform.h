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
	bool requireUniformScale = true;

	vec3 LocalPosition() const { return localPosition; }
	void SetLocalPosition(vec3 pos);
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
	void SetScale2D(vec2 scale, bool allowFlip = false);
	inline vec2 Position2D() const { return (vec2)Position(); }
	inline float Angle() const { return glm::eulerAngles(Rotation()).z; } // this is in radians.
	inline vec2 Scale2D() const { return (vec2)Scale(); }

	void SetPosition(vec3 pos);
	void SetRotation(quat rot);
	void SetScale(vec3 scale);
	inline vec3 Position() const { return vec3(model[3]); }
	inline quat Rotation() const { return quat(model); }
	vec3 Scale() const;
	mat4 InverseModel() const; // probably only used by camera
	void SetParent(Transform* newParent);
	vector<Transform*> GetChildren() const;
	string GetPath() const; // could also be called to_string
	bool IsDescendantOf(const Transform& putativeAncestor) const;
	inline bool IsTransformFullyEnabled() const { return Enabled() && (!HasParent() || Parent().IsTransformFullyEnabled()); }
	Transform& Root();
	inline bool HasParent() const { return UuidCreator::IsInitialized(parentID); }
	inline bool IsStatic() const { return isStatic; }
	void SetStatic(bool becomeStatic);
	inline mat4 Model() const { return model; }

	vec3 ToWorldSpace(vec3 position, bool isPosition) const;
	vec2 ToWorldSpace(vec2 position2D, bool isPosition) const;
	vec3 ToLocalSpace(vec3 position, bool isPosition) const;
	vec2 ToLocalSpace(vec2 position2D, bool isPosition) const;

	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;
	int InitOrder() override { return -2000; }

private:
	uuid parentID;
	//bool isUpToDate = false; // i.e. caches are up to data
	mat4 model = mat4(1.0f); // cached value
	vector<Transform*> children;
	vec3 localPosition = vec3(0.0f);
	quat localRotation = glm::NoRotation();
	vec3 localScale = vec3(1.0f);
	bool isStatic = false;

	void OnStart() override { unique = true; }
	void OnDestroy() override;
	mat4 LocalModel() const;
	mat4 InverseLocalModel() const; // probably only used by camera
	void SetLocalDataUsingMatrix(const mat4& transform);
	Transform* TryParent() const;
	inline Transform& Parent() const { return Entity::GetComponent<Transform>(parentID); }
	void CacheModel(const mat4& m);
	mat4 CalcModel() const;

};


