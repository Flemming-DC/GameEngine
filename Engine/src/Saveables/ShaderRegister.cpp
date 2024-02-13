#include "ShaderRegister.h"
#include "ListTools.h"
#include <memory>

std::vector<Shader> ShaderRegister::shaders = {};
std::unordered_map<uuids::uuid, int> ShaderRegister::indexByID = {};


uuids::uuid ShaderRegister::MakeShader(const std::string& filePath)
{
	shaders.emplace_back(filePath);
	indexByID[shaders.back().GetID()] = shaders.size() - 1;
	return shaders.back().GetID();
}

uuids::uuid ShaderRegister::Add(const Shader& shader)
{
	shaders.emplace_back(shader);
	indexByID[shaders.back().GetID()] = shaders.size() - 1;
	return shaders.back().GetID();
}

void ShaderRegister::Remove(const uuids::uuid& shaderID)
{
	if (!Tools::ContainsKey_unordered(indexByID, shaderID))
		RaiseError("Trying to reomve an object, which isn't in the register.");
	shaders.erase(shaders.begin() + indexByID[shaderID]);
	Tools::RemoveKey_unordered(indexByID, shaderID);
}

const Shader& ShaderRegister::Get(const uuids::uuid& shaderID)
{
	return shaders[indexByID[shaderID]];
}

