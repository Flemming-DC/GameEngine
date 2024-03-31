#pragma once
#include "Engine.h"

class Editor : public Dynamic
{
public:
	//static void OnSceneChanged();

private:
	glm::vec3 cameraPos = glm::vec3(0.0f);
	const glm::quat cameraRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 cameraScale = glm::vec3(1.0f);
	float moveSpeed = 1.0f; // camera
	const float scrollSpeed = 4.0f / 60.0f; // camera
	const float maxScale = 100;

	void OnEditorStart() override;
	void OnEditorUpdate() override;

	void SceneEditor();
	void UpdateCamera();
};

