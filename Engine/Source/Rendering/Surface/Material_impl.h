#pragma once
#include "Material.h"
#include "ListTools.h"
#include <string>


template <typename T>
inline T Material::GetUniform(const std::string& uniformName) const
{
	Assert(Tools::ContainsKey(uniformValuesByName, uniformName),
		"The uniform " + uniformName + " is not recognized.\n"
		"The material contains the following uniforms\n" +
		logger::to_string(Tools::GetKeys(uniformValuesByName))
		);
	
	Assert(uniformValuesByName.at(uniformName).type() == typeid(T),
		"The uniform " + uniformName + " exists, but it doesn't have "
		"the type specified by the caller of GetUniform.\n"
		"The material contains the following uniforms\n" +
		logger::to_string(Tools::GetKeys(uniformValuesByName))
		);

	T uniform = std::any_cast<T>(uniformValuesByName.at(uniformName));
	return uniform;
}



