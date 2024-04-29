#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include <memory>
#include "Event.h"
#include "Delay.h"
#include "StringTools.h"
#include "EngineFiles.h"

class Scene
{
public:
	static Event<Scene&> onStart;
	static Event<Scene&> onEnd; 

	template <typename SceneType> static void Activate();
	//static void Activate(Scene* scenePtr);
	static void ActivateImmediately(Scene* scenePtr);
	static void MakeBlankSceneFile(std::string name); // evt. temp
	void ShutDown();
	static void Save(); // save activeScene to file
	static Scene& GetActiveScene() { return *activeScene; }
	virtual void PurelyManualSetup() { RaiseError("not implemented"); };
	static void ReloadImmediately();
	static void SetFirstScene(std::unique_ptr<Scene> firstScene);
protected:
	virtual void ManualSetup() = 0;

private:
	static uuids::uuid activeSceneID;
	static std::unique_ptr<Scene> activeScene;
	uuids::uuid id;

	void Load(); // load from file
	std::string Path() { return EngineFiles::Scenes + Tools::TypeName(*this) + ".yml"; } // evt. store path rather than name
};


template <typename SceneType> static void Scene::Activate()
{
	static_assert(std::is_base_of<Scene, SceneType>::value,
		"Scene::Activate can only activate Scenes, not other types");

	Delay::ToFrameEnd([]() { ActivateImmediately(new SceneType()); });
}

