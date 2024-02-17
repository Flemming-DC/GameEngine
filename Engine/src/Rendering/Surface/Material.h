#pragma once
#include <string>
#include <map>
#include <any>
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "Register.h"
#include "UuidCreator.h"

// evt. take Shader and Textures as input by class rather than by path
// when scene uniforms get introduced, then we need to adjust the material class (binding and checking) to account for it
class Material
{
public:
	struct MissingUniform {};
	static Register<Material> register_;

	Material() {}
	Material(const Shader& shader_, const std::map<std::string, std::any>& uniformValuesByName_) 
		{ Setup(shader_, uniformValuesByName_); }

	void Setup(const Shader& shader_, const std::map<std::string, std::any>& uniformValuesByName_);

	void Bind(bool allowMissingUniforms = false);
	static void UnBind();
	// Sets the uniform internally in the material. 
	// nb: it wont be sent to the GPU until the renderer issues a draw call.
	void SetUniform(const std::string& name, std::any value);
	uuids::uuid GetID() const { return id; }

	template <typename T> inline T GetUniform(const std::string& uniformName);
	

private:
	Shader shader;
	uuids::uuid id;
	std::map<std::string, Texture*> texturesByName; // this is a subset of uniforms by name
	std::map<std::string, std::any> uniformValuesByName;

	void CheckUniforms();
	void SetupTexturesByName();
};


#include "Material_impl.h"
