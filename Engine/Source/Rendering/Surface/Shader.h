#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <stduuid/uuid.h>
#include "glm/glm.hpp"
#include "Register.h"
#include "UuidCreator.h"

// check if file exists
struct ShaderStrings
{
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader
{
public:
	static Register<Shader> register_;

	Shader() { }
	Shader(const std::string& filePath) { Setup(filePath); }
	void ShutDown();
	void Setup(const std::string& filePath);
	void Bind() const;
	static void UnBind();
	void SetUniform(const std::string& name, std::any value);
	std::string GetPath() const { return path; }
	std::unordered_map<std::string, std::string> GetUniformTypesByName() const { return uniformTypesByName; }
	int GetTextureSlot(std::string uniformName) { return textureSlotsByName[uniformName]; }
	inline uuids::uuid GetID() const { return id; }
	std::string to_string() const;

private:
	uuids::uuid id; 
	std::string path = "";
	unsigned int openGLid = 0;
	std::unordered_map<std::string, int> locationByName;
	std::unordered_map<std::string, int> idByFilePath;
	std::unordered_map<std::string, std::string> uniformTypesByName;
	std::unordered_map<std::string, int> textureSlotsByName;

	void SetUniformInt(const std::string& name, int i);
	void SetUniformVec4(const std::string& name, const glm::vec4& vector4);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
	void SetUniformTexture(const std::string& name);

	void CheckFilePath(const std::string& filePath);
	int GetUniformLocation(const std::string& name);
	ShaderStrings ParseShader(const std::string& filePath); // splits the shader into a vertex shader and a fragment shader
	void FindUniforms(const std::string& shaderSource);
	unsigned int CompileShader(unsigned int type, const std::string& source); // returns id for subShader
	unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader); // returns id for full shader
};
