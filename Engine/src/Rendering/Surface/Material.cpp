#include "Material.h"
#include <string>
#include "Shader.h"
#include <map>
#include "ErrorChecker.h"
#include "StringTools.h"
#include "ListTools.h"
#include "OtherTools.h"


void Material::Setup(
    const Shader& shader_,
    const std::map<std::string, std::any>& uniformValuesByName_)
{
    shader = shader_;
    uniformValuesByName = uniformValuesByName_;
    CheckUniforms();
    SetupTexturesByName();
    Bind(true);
}

Material::~Material()
{
    //  check if there are other references to the textures. If not, then delete them.
    //for (auto texture : textures)
        //delete texture;
}

void Material::SetUniform(const std::string& name, std::any value)
{
    if (!Tools::ContainsKey(uniformValuesByName, name))
        RaiseError(
            "The uniform " + name + " is not recognized.\n"
            "The material contains the following uniforms\n" + 
            Tools::to_string(Tools::GetKeys(uniformValuesByName))
            );
    uniformValuesByName[name] = value;
}

void Material::Bind(bool allowMissingUniforms)
{
    using namespace std;
    for (auto pair : texturesByName)
        pair.second->Bind(shader.GetTextureSlot(pair.first));
    shader.Bind();

    for (const auto& pair : uniformValuesByName)
    {
        string name = pair.first;
        any value = pair.second;
        bool hasValue = value.has_value() && !Tools::IsOfType<MissingUniform>(value);
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
        if (!Tools::ContainsKey(uniformValuesByName, name))
            RaiseError(
                "Material lacks a uniforms named " + name + ", even though this uniform is a part of the shader.");
    }
}

void Material::SetupTexturesByName()
{
    for (const auto& pair : uniformValuesByName)
    {
        try
        {
            Texture* texturePtr = std::any_cast<Texture*>(pair.second);
            if (texturePtr)
                texturesByName[pair.first] = texturePtr;
        }
        catch (const std::bad_any_cast& _) { (void)_; } // the expression (void)_ prevents an unused variable warning
    }
}