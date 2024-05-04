#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include "ErrorChecker.h"
#include "StringTools.h"
#include "logger.h"

class Dynamic
{
public:

	template <typename... Args>
	static void Setup()
	{
		(..., Setup_Internal<Args>()); // (..., expr) expands to a repetition of expr for each arg
	}

	
	static bool ShutDown(Dynamic& dynamic);
	static void Clear();

	static void CallOnEditorStart();
	static void CallOnGameStart();
	static void CallOnUpdate();
	static void CallOnEditorUpdate();
	static void CallOnGameEnd();
	static void CallOnEditorEnd();

private:
	static std::vector<std::unique_ptr<Dynamic>> dynamics;

	inline virtual void OnEditorStart() {}
	inline virtual void OnGameStart() {}
	inline virtual void OnUpdate() {}
	inline virtual void OnEditorUpdate() {}
	inline virtual void OnGameEnd() {}
	inline virtual void OnEditorEnd() {}


	template <typename DynamicType>
	static DynamicType& Setup_Internal()
	{
		static_assert(std::is_base_of<Dynamic, DynamicType>::value,
			"Dynamic::Setup can only handle subclasses of Dynamic, not other types.");
		if (typeid(DynamicType) == typeid(Dynamic))
			RaiseError("Dynamic::Setup can only handle subclasses of Dynamic, not the base class Dynamic.");

		dynamics.push_back(std::make_unique<DynamicType>());
		Dynamic* ptr = dynamics.back().get();
		DynamicType* afterCast = dynamic_cast<DynamicType*>(ptr);
		if (afterCast == nullptr)
			RaiseError("dynamic_cast failed for Dynamic::Add<" + Tools::TypeName<DynamicType>() + ">()");
		return *afterCast;
	}
};

