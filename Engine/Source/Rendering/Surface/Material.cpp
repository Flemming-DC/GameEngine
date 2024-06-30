#include "Material.h"
#include <string>
#include <map>
#include "Shader.h"
#include "ErrorChecker.h"
#include "StringTools.h"
#include "ListTools.h"
#include "OpenGlSetup.h"
#include "YmlTools.h"
#include "EngineLiterals.h"

Register<Material> Material::register_;
Naming Material::naming;

void Material::Setup(string name, const Shader& shader_, const map_uo<string, std::any>& uniformValuesByName_)
{
    Assert(OpenGlSetup::Initialized(),
        "Material cannot be setup before OpenGlSetup::Setup() is called.");
    Deny(UuidCreator::IsInitialized(id),
        "Material is already initialized");
    if (!naming.Contains(name))
        naming.AddWithSuffix(name, UuidCreator::MakeID());
    id = naming.at(name);
    shader = shader_;
    uniformValuesByName = uniformValuesByName_;
    InDebug(CheckUniforms());
    SetupTexturesByName();
    Bind(true);
}


void Material::SetUniform(const string& name, const std::any& value)
{
    Assert(UuidCreator::IsInitialized(id),
        "You cannot set a uniform upon an uninitialized Material");
    Assert(Tools::ContainsKey(uniformValuesByName, name),
        "The uniform " + name + " is not recognized.\n"
        "The material contains the following uniforms\n" + 
        logger::to_string(Tools::GetKeys(uniformValuesByName))
        );
    Assert(uniformValuesByName.at(name).type() == value.type() 
        || uniformValuesByName.at(name).type() == typeid(MissingUniform),
        "Trying to change the type of the (non-missing) uniform " + name
        );

    uniformValuesByName[name] = value;
    
    if (value.type() == typeid(Texture*))
    {
        Texture* texturePtr = std::any_cast<Texture*>(value);
        Assert(texturePtr, "texturePtr is null");
        texturesByName[name] = texturePtr;
    }
}

void Material::SetTexture(const string& uniformName, uuid texID)
{
    Assert(Texture::register_.Contains(texID),
        "Trying to set uniform " + uniformName + " with a textureID ", texID, 
        " that does exist in the register. The textue name is " + Texture::naming.at(texID));
    Texture& tex = Texture::register_.Get(texID);
    SetUniform(uniformName, &tex);
}


void Material::SetColor(vec4 color)
{
    SetUniform(Literals::u_color, color);
}
glm::vec4 Material::Color()
{
    return GetUniform<vec4>(Literals::u_color);
}
void Material::IncrementColor(vec4 colorDelta)
{
    vec4 color = glm::clamp(Color() + colorDelta, vec4(0.0f), vec4(1.0f));
    SetUniform(Literals::u_color, color);
}


void Material::Bind(bool allowMissingUniforms)
{
    Assert(UuidCreator::IsInitialized(id),
        "You cannot bind an uninitialized Material");

    using namespace std;
    for (const auto& [name, tex] : texturesByName)
        tex->Bind(shader.GetTextureSlot(name));
    shader.Bind();

    for (const auto& [name, value] : uniformValuesByName)
    {
        bool hasValue = value.has_value() && value.type() != typeid(MissingUniform);
        if (allowMissingUniforms && !hasValue)
            continue;
        Assert(hasValue,
            "Material is being bound, while uniform lacks a value:\n"
            "    uniform = " + name + "\n"
            "    shader = " + shader.GetPath()
        );
        
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
    for (const auto& [name, _] : uniformValuesByName)
    {
        if (!Tools::ContainsKey(shader.GetUniformTypesByName(), name))
            Warning(
                "Material contains a uniform named " + name + ", even though this uniform doesn't exist in the shader.");
    }
    for (const auto& [name, _] : shader.GetUniformTypesByName())
    {
        // we add "u_MVP": MissingUniform ourselves, since its an implementation detail.
        if (name == "u_MVP" && !Tools::ContainsKey(uniformValuesByName, name))
            uniformValuesByName["u_MVP"] = MissingUniform();
        else 
        {
            Assert(Tools::ContainsKey(uniformValuesByName, name),
                "Material lacks a uniforms named " + name + ", even though this uniform is a part of the shader.");
        }   
    }
}

void Material::SetupTexturesByName()
{
    texturesByName.clear();
    for (const auto& [name, value] : uniformValuesByName)
    {
        if (value.type() == typeid(Texture*))
        {
            Texture* texturePtr = std::any_cast<Texture*>(value);
            Assert(texturePtr, "texturePtr is null");
            texturesByName[name] = texturePtr;
        } 
        else { Deny(name == Literals::u_textureSampler, "u_textureSampler should be a texture"); }
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

