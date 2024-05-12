#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include <memory>
#include "Event.h"
#include "Delay.h"
#include "StringTools.h"
#include "EngineLiterals.h"

class Scene
{
public:
	static Event<Scene&> onStart;
	static Event<Scene&> onEnd; 

	Scene(std::string path) : path(path) {}
	static void Activate(std::string path);
	static void ActivateImmediately(std::string path);
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
	virtual void OnStart() {};

private:
	static uuids::uuid activeSceneID;
	static std::unique_ptr<Scene> activeScene;
	uuids::uuid id;
	std::string path;

	void Load(); // load from file
	std::string Path() { return path; };// { return Literals::Scenes + name + ".yml"; } // evt. store path rather than name
};


template <typename SceneType> static void Scene::Activate()
{
	static_assert(std::is_base_of<Scene, SceneType>::value,
		"Scene::Activate can only activate Scenes, not other types");

	Delay::ToFrameEnd([]() { 
		std::string name = Tools::TypeName<SceneType>();
		ActivateImmediately(new SceneType(name)); 
		});
}

