#include "Material.h"
#include <string>
#include <map>
#include "Shader.h"
#include "ErrorChecker.h"
#include "StringTools.h"
#include "ListTools.h"
#include "OpenGlSetup.h"

Register<Material> Material::register_;

void Material::Setup(
    const Shader& shader_,
    const std::map<std::string, std::any>& uniformValuesByName_)
{
    if (!OpenGlSetup::Initialized())
        RaiseError("Material cannot be setup before OpenGlSetup::Setup() is called.");
    if (UuidCreator::IsInitialized(id))
        RaiseError("Material is already initialized");
    id = UuidCreator::MakeID();
    shader = shader_;
    uniformValuesByName = uniformValuesByName_;
    CheckUniforms();
    SetupTexturesByName();
    Bind(true);
}


void Material::SetUniform(const std::string& name, std::any value)
{
    if (!Tools::ContainsKey(uniformValuesByName, name))
        RaiseError(
            "The uniform " + name + " is not recognized.\n"
            "The material contains the following uniforms\n" + 
            logger::to_string(Tools::GetKeys(uniformValuesByName))
            );
    uniformValuesByName[name] = value;
}

void Material::Bind(bool allowMissingUniforms)
{
    if (!UuidCreator::IsInitialized(id))
        RaiseError("You cannot bind an uninitialized Material");

    using namespace std;
    for (auto pair : texturesByName)
        pair.second->Bind(shader.GetTextureSlot(pair.first));
    shader.Bind();

    for (const auto& pair : uniformValuesByName)
    {
        string name = pair.first;
        any value = pair.second;
        bool hasValue = value.has_value() && value.type() != typeid(MissingUniform);
        
        if (!hasValue)
        {
            if (allowMissingUniforms)
                continue;
            RaiseError(
                "Material is being bound, while uniform lacks a value:\n"
                "    uniform = " + name + "\n"
                "    shader = " + shader.GetPath());
        }
        
        shader.SetUniform(name, value);
    }

}

void Material::UnBind()
{
    Shader::UnBind();
    Texture::UnBind();
    /*
    shader.UnBind();
    for (auto pair : texturesByName)
        pair.second->UnBind();
    */
}

void Material::CheckUniforms()
{
    for (const auto& pair : uniformValuesByName)
    {
        std::string name = pair.first;
        if (!Tools::ContainsKey_unordered(shader.GetUniformTypesByName(), name))
            Warning(
                "Material contains a uniform named " + name + ", even though this uniform doesn't exist in the shader.");
    }
    for (const auto& pair : shader.GetUniformTypesByName())
    {
        std::string name = pair.first;
        // we add "u_MVP": MissingUniform ourselves, since its an implementation detail.
        if (name == "u_MVP" && !Tools::ContainsKey(uniformValuesByName, name))
            uniformValuesByName["u_MVP"] = MissingUniform(); 
        else if (!Tools::ContainsKey(uniformValuesByName, name))
            RaiseError(
                "Material lacks a uniforms named " + name + ", even though this uniform is a part of the shader.");
    }
}

void Material::SetupTexturesByName()
{
    for (const auto& pair : uniformValuesByName)
    {
        if (pair.second.type() == typeid(Texture*))
        {
            Texture* texturePtr = std::any_cast<Texture*>(pair.second);
            if (texturePtr)
                texturesByName[pair.first] = texturePtr;
        }

    }
}

std::string Material::to_string() const
{
    std::string newline = "\n    ";
    std::string out = "Material:" + newline;
    out += "id: " + logger::to_string(id) + newline;
    out += "shader: " + Tools::Replace(shader.to_string(), "\n", newline) + newline;
    for (const auto& pair : uniformValuesByName)
    {
        if (Tools::ContainsKey(texturesByName, pair.first)) //check if its a texture
        {
            std::string textureStr = texturesByName.at(pair.first)->to_string();
            out += "texture: " + Tools::Replace(textureStr, "\n", newline) + newline;
        }
        else
            out += "non-texture uniform: " + pair.first + newline;
    }
    return out;
}