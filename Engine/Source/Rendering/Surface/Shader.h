#pragma once
#include <any>
#include "glm/glm.hpp"
#include "Register.h"
#include "UuidCreator.h"

// check if file exists
struct ShaderStrings
{
	std::string vertexShader;
	std::string fragmentShader;
};
enum class UniformType { int_, vec4_, mat4_, sampler2D_, };

class Shader
{
public:
	Shorts;
	static Register<Shader> register_;

	Shader() { }
	Shader(const string& filePath) { Setup(filePath); }
	void ShutDown();
	void Setup(const string& filePath);
	void Bind() const;
	static void UnBind();
	void SetUniform(const string& name, std::any value);
	const string& GetPath() const { return path; }
	map_uo<string, UniformType> GetUniformTypesByName() const { return uniformTypesByName; }
	int GetTextureSlot(const string& uniformName) { return textureSlotsByName[uniformName]; }
	inline uuid GetID() const { return id; }
	string to_string() const;

private:
	uuid id; 
	string path = "";
	uint openGLid = 0;
	map_uo<string, int> locationByName;
	map_uo<string, int> idByFilePath;
	map_uo<string, UniformType> uniformTypesByName;
	map_uo<string, int> textureSlotsByName;

	void SetUniformInt(const string& name, int i);
	void SetUniformVec4(const string& name, const vec4& vector4);
	void SetUniformMat4(const string& name, const mat4& matrix);
	void SetUniformTexture(const string& name);

	void CheckFilePath(const string& filePath);
	int GetUniformLocation(const string& name);
	ShaderStrings ParseShader(const string& filePath); // splits the shader into a vertex shader and a fragment shader
	void FindUniforms(const string& shaderSource);
	unsigned int CompileShader(uint type, const string& source); // returns id for subShader
	unsigned int CreateShaderProgram(const string& vertexShader, const string& fragmentShader); // returns id for full shader
};
