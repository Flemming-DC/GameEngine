#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include <memory>
#include "Register.h"
#include "Event.h"
#include "Renderer.h"
#include "Delay.h"


class Scene
{
public:
	static Event<Scene&> onStart;
	static Event<Scene&> onEnd; 

	static void MakeBlankSceneFile(std::string name);
	template <typename SceneType> static void Activate();
	void ShutDown();
	void Save(); // save to file
	static Scene& GetActiveScene() { return *activeScene; }
	virtual void PurelyManualSetup() = 0;

protected:
	std::vector<uuids::uuid> entityIDs; // is this dublicate info? They are also in the register. 

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
		"Scene::Make can only makes Scenes, not other types");
	
	Delay::ToFrameEnd([]()
	{
		if (activeScene)
			activeScene->ShutDown();

		activeScene = std::make_unique<SceneType>();
		Renderer::SetupGrid2D(0.25f); // if is_editor

		activeScene->Load();
		activeScene->ManualSetup();
		onStart.Invoke(*activeScene);
	});
	
}