#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ErrorChecker.h"
#include "StringTools.h"


class Dynamic
{
public:
	template <typename initializableType>
	static initializableType& Add()
	{
		static_assert(std::is_base_of<Dynamic, initializableType>::value,
			"Dynamic::Add can only add Initializables, not other types");

		initializables.emplace_back(std::make_unique<initializableType>());
		Dynamic* ptr = initializables.back().get();
		initializableType* afterCast = dynamic_cast<initializableType*>(ptr);
		if (afterCast == nullptr)
			RaiseError("dynamic_cast failed for Dynamic::Add<" + Tools::TypeName<initializableType>() + ">()");
		return *afterCast;
	}
	
	static bool Remove(Dynamic& initializable);
	static void Clear();

	static void CallOnEngineStart();
	static void CallOnGameStart();
	static void CallOnUpdate();
	static void CallOnGameEnd();
	static void CallOnEngineEnd();


private:
	static std::vector<std::unique_ptr<Dynamic>> initializables;

	inline virtual void OnEngineStart() {}
	inline virtual void OnGameStart() {}
	inline virtual void OnUpdate() {}
	inline virtual void OnGameEnd() {}
	inline virtual void OnEngineEnd() {}

};

