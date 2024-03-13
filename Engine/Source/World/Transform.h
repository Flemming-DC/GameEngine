#pragma once
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include "Component.h"
#include "YAML.h"

// evt. use local model as the authoritative parameter with model cached and everything else generated from those two.
// evt. apply to caching in self and ancestors. use set local / glocal parameters instead of direct exposure of local
// parameters to control updates of the caching
// evt. use ToWorldSpace to calculate world position, rotation and scale
// evt. enforce that localScale cannot be zero or negative
// toggle enforce proportions and use this by the circle collider
class Transform : public Component
{
public:
	glm::vec3 localPosition = glm::vec3(0.0f); // show in inspector
	glm::quat localRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // show in inspector
	glm::vec3 localScale = glm::vec3(1.0f); // show in inspector

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetScale() const;
	glm::mat4 GetModel() const;
	glm::mat4 GetInverseModel() const; // probably only used by camera
	Transform* GetParent() const;
	void SetParent(Transform* newParent);
	std::vector<Transform*> GetChildren() const;
	std::string GetPath() const; // could also be called to_string
	// get forward, backward, right, left, up, down

	//glm::vec2 As2D
	//glm::vec2 LocalPosition2D() { return glm::vec2(localPosition.x, localPosition.y); }

	glm::vec3 ToWorldSpace(glm::vec3 position, bool isPosition) const;
	glm::vec2 ToWorldSpace(glm::vec2 position2D, bool isPosition) const;
	glm::vec3 ToLocalSpace(glm::vec3 position, bool isPosition) const;
	glm::vec2 ToLocalSpace(glm::vec2 position2D, bool isPosition) const;

	void Save(YAML::Node& node) const override;
	void Load(const YAML::Node& node) override;

private:
	//uuids::uuid parentID;
	Transform* parent = nullptr;
	bool isUpToDate = false; // i.e. caches are up to data
	//glm::mat4 model = glm::mat4(1.0f); // cached value
	std::vector<Transform*> children;

	void OnDestroy() override;
	glm::mat4 GetLocalModel() const;
	glm::mat4 GetInverseLocalModel() const; // probably only used by camera
	void SetLocalDataUsingTransform(const glm::mat4& transform); 

};


