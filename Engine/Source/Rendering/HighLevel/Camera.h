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
	static mat4 ProjectionView(vec3 cameraPos, quat cameraRot, vec3 cameraScale);
	mat4 ProjectionView() const { return Projection() * View(); };
	mat4 Projection() const;
	mat4 View() const; 
	static Camera& Current();
	void SetCameraAsCurrent() { currentCamera = this; }
	int InitOrder() override { return -1200; }

private:
	static Camera* currentCamera;
	float nearClipping = 0.0f;
	float moveSpeed = 1.0f;
	float scrollSpeed = 4.0f;
	mat4 projection = glm::mat4(1.0f);
	bool isProjectionInitialized = false;

	void OnStart() override;
	void OnDestroy() override;

	static mat4 MakeOrthoProjection(float scale = 1.0f, float nearClipping = 0.0f, float farClipping = 1000.0f);
};



