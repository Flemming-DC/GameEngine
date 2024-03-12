#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ErrorChecker.h"
#include "StringTools.h"


class Initializable
{
public:
	template <typename initializableType>
	static initializableType& Add()
	{
		static_assert(std::is_base_of<Initializable, initializableType>::value,
			"Initializable::Add can only add Initializables, not other types");

		initializables.emplace_back(std::make_unique<initializableType>());
		Initializable* ptr = initializables.back().get();
		initializableType* afterCast = dynamic_cast<initializableType*>(ptr);
		if (afterCast == nullptr)
			RaiseError("dynamic_cast failed for Initializable::Add<" + Tools::TypeName<initializableType>() + ">()");
		return *afterCast;
	}
	
	static bool Remove(Initializable& initializable);
	static void Clear();

	static void CallOnEngineStart();
	static void CallOnGameStart();

	static void CallOnGameEnd();
	static void CallOnEngineEnd();


private:
	static std::vector<std::unique_ptr<Initializable>> initializables;

	virtual void OnEngineStart() {}
	virtual void OnGameStart() {}

	virtual void OnGameEnd() {}
	virtual void OnEngineEnd() {}

};

