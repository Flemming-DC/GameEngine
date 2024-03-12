#pragma once
#include <unordered_map>
#include <memory>


class Initializable
{
public:
	template <typename initializableType>
	static unsigned int Add()
	{
		static_assert(std::is_base_of<Initializable, initializableType>::value,
			"Initializable::Add can only add Initializables, not other types");

		maxId++;
		initializables[maxId] = std::make_unique<initializableType>();
		return maxId;
		/*
		componentsByEntity[id].push_back(std::make_unique<ComponentType>());
		Component* ptr = componentsByEntity[id].back().get();
		ComponentType* afterCast = dynamic_cast<ComponentType*>(ptr);
		if (afterCast == nullptr)
			RaiseError("dynamic_cast failed for " + name + ".AddComponent<" + Tools::to_string<ComponentType>() + ">()");
		afterCast->OnAddComponent(id, node);
		componentsByID[afterCast->id] = ptr;
		return *afterCast;
		*/
	}
	
	
	
	static bool Remove(unsigned int id);
	static void Clear();

	static void CallOnEngineStart();
	static void CallOnGameStart();
	//static void CallOnSceneStart();

	//static void CallOnSceneEnd();
	static void CallOnGameEnd();
	static void CallOnEngineEnd();


private:
	static std::unordered_map<unsigned int, std::unique_ptr<Initializable>> initializables;
	static unsigned int maxId;

	virtual void OnEngineStart() {}
	virtual void OnGameStart() {}
	// virtual void OnSceneStart() {}

	// virtual void OnSceneEnd() {}
	virtual void OnGameEnd() {}
	virtual void OnEngineEnd() {}

};

