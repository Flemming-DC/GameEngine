#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Transform.h"
#include "Component.h"


// evt. cache projection and introduce GetProjectionView 
class Camera : public Component
{
public:
	Shorts
	void SetToOrthographic(float scale = 1.0f, float nearClipping = 0.0f, float farClipping = 1000.0f);
	//glm::mat4 GetProjectionView() const; // returns projectionMatrix times viewMatrix

	static mat4 ProjectionView(vec3 cameraPos, quat cameraRot, vec3 cameraScale);
	mat4 ProjectionView() { return GetProjection() * GetView(); };
	mat4 GetProjection() const;
	mat4 GetView(); //const
	static Camera& GetCurrent();
	void SetCameraAsCurrent() { currentCamera = this; }

private:
	static Camera* currentCamera;
	float nearClipping = 0.0f;
	float moveSpeed = 1.0f;
	float scrollSpeed = 4.0f;
	mat4 projection = glm::mat4(1.0f);
	bool isProjectionInitialized = false;

	void OnStart() override;
	void OnUpdate() override;
	void OnDestroy() override;

};



