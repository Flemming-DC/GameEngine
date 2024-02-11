#pragma once
#include "Material.h"
#include "OtherTools.h"
#include "ListTools.h"
#include <string>


template <typename T>
inline T Material::GetUniform(const std::string& uniformName)
{
	if (!Tools::ContainsKey(uniformValuesByName, uniformName))
		RaiseError(
			"The uniform " + uniformName + " is not recognized.\n"
			"The material contains the following uniforms\n" +
			Tools::to_string(Tools::GetKeys(uniformValuesByName))
		);
		
	T* uniformPtr = Tools::TryAnyCast<T>(uniformValuesByName[uniformName]);
	if (uniformPtr == nullptr)
		RaiseError(
			"The uniform " + uniformName + " exists, but it doesn't have "
			"the type specified by the caller of GetUniform.\n"
			"The material contains the following uniforms\n" +
			Tools::to_string(Tools::GetKeys(uniformValuesByName))
		);
		
	return *uniformPtr;
}



