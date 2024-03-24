#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include <memory>
#include "Register.h"
#include "Event.h"
#include "Renderer.h"
#include "Delay.h"
#include "StringTools.h"


class Scene
{
public:
	static Event<Scene&> onStart;
	static Event<Scene&> onEnd; 

	static void MakeBlankSceneFile(std::string name); // evt. temp
	template <typename SceneType> static void Activate();
	template <typename SceneType> static void ActivateImmediately(); // only used by the engine and only for the starting scene
	void ShutDown();
	static void Save(); // save activeScene to file
	static Scene& GetActiveScene() { return *activeScene; }
	virtual void PurelyManualSetup() = 0;

protected:
	virtual void ManualSetup() = 0;

private:
	static uuids::uuid activeSceneID;
	static std::unique_ptr<Scene> activeScene;
	uuids::uuid id;

	void Load(); // load from file
	std::string Path() { return "res/Scenes/" + Tools::TypeName(*this) + ".yml"; } // evt. store path rather than name
};


template <typename SceneType> static void Scene::Activate()
{
	static_assert(std::is_base_of<Scene, SceneType>::value,
		"Scene::Activate can only activate Scenes, not other types");
	
	Delay::ToFrameEnd([]() { ActivateImmediately<SceneType>(); });
	
}


template <typename SceneType> static void Scene::ActivateImmediately()
{
	static_assert(std::is_base_of<Scene, SceneType>::value,
		"Scene::ActivateImmediately can only activate Scenes, not other types");

	if (activeScene)
		activeScene->ShutDown();

	activeScene = std::make_unique<SceneType>();
	Renderer::SetupGrid2D(0.25f); // if is_editor

	activeScene->Load();
	activeScene->ManualSetup();
	onStart.Invoke(*activeScene);
}

