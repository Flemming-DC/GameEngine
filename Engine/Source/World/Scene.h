#pragma once
#include <string>
#include <vector>
#include <stduuid/uuid.h>
#include <memory>
#include "Event.h"
#include "Delay.h"
#include "Naming.h"
#include "StringTools.h"
#include "EngineLiterals.h"

class Scene
{
public:
	Shorts;
	static Event<Scene&> onStart;
	static Event<Scene&> onEnd;
	static Naming naming;

	static void Activate(string name); // used by game, not engine
	static void ActivateImmediately(string name); // engine only
	static void MakeBlankSceneFile(string name); 
	void ShutDown(); // engine only
	static void Save(); // save activeScene to file
	static void Reload();
	static void ReloadImmediately(); // engine & editor only
	static Scene& GetActiveScene() { return *activeScene; }

protected:
	virtual void OnStart() {};

private:
	static unique_ptr<Scene> activeScene;
	uuid id;

	Scene(uuid id) : id(id) {}
	void Load(); // load from file
};

