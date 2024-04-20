#pragma once
#include <string>
#include <map>
#include <any>
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "Register.h"
#include "UuidCreator.h"
#include "Naming.h"


// evt. take Shader and Textures as input by class rather than by path
// when scene uniforms get introduced, then we need to adjust the material class (binding and checking) to account for it
class Material
{
	Shorts;
public:
	struct MissingUniform {};
	static Register<Material> register_;
	static Naming naming;

	Material() { }
	Material(string name, const Shader& shader_, const map_uo<string, std::any>& uniformValuesByName_)
		{ Setup(name, shader_, uniformValuesByName_); }

	void Setup(string name, const Shader& shader_, const map_uo<string, std::any>& uniformValuesByName_);

	void Bind(bool allowMissingUniforms = false);
	static void UnBind();
	// Sets the uniform internally in the material. 
	// nb: it wont be sent to the GPU until the renderer issues a draw call.
	void SetUniform(const string& name, std::any value);
	inline uuid GetID() const { return id; }
	inline static vector<string> Names() { return naming.Names(); }

	template <typename T> inline T GetUniform(const std::string& uniformName);
	string to_string() const;

private:
	Shader shader;
	uuid id;
	map_uo<string, Texture*> texturesByName; // this is a subset of uniforms by name
	map_uo<string, std::any> uniformValuesByName;

	void CheckUniforms();
	void SetupTexturesByName();
};


#include "Material_impl.h"
