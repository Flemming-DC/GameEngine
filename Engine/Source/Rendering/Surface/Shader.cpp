#include "Shader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <any>
#include <filesystem>
#include "OpenGlExternal.h"
#include "OpenGlError.h"
#include "OpenGlSetup.h"
#include "OpenGLidChecker.h"
#include "StringTools.h"
#include "Dynamic.h"

// ---------------- public ----------------
Register<Shader> Shader::register_;



void Shader::Setup(const std::string& filePath)
{
    Assert(OpenGlSetup::Initialized(),
        "Shader cannot be setup before OpenGlSetup::Setup() is called.");
    Deny(UuidCreator::IsInitialized(id),
        "Shader is already initialized");
    id = UuidCreator::MakeID();
    path = filePath;
    if (idByFilePath.find(filePath) != idByFilePath.end())
        openGLid = idByFilePath[filePath];
    else
    {
        CheckFilePath(filePath);
        ShaderStrings shaderStruct = ParseShader(filePath);
        openGLid = CreateShaderProgram(shaderStruct.vertexShader, shaderStruct.fragmentShader);
        FindUniforms(filePath);
        idByFilePath[filePath] = openGLid;
    }
    glCall(glUseProgram(openGLid));
    OpenGLidChecker::Add(Tools::TypeName(*this), openGLid);
}

void Shader::ShutDown()
{
    //if (UuidCreator::IsInitialized(id) != (openGLid != 0))
    //    RaiseError("UuidCreator::IsInitialized(id) doesn't match (openGLid != 0)");
    Assert(UuidCreator::IsInitialized(id) == (openGLid != 0),
        "UuidCreator::IsInitialized(id) doesn't match (openGLid != 0)");
    if (!UuidCreator::IsInitialized(id))
        return;
    glCall(glDeleteProgram(openGLid));
    OpenGLidChecker::Remove(Tools::TypeName(*this), openGLid);
}

void Shader::Bind() const
{
    Assert(UuidCreator::IsInitialized(id),
        "You cannot bind an uninitialized Shader");

    glCall(glUseProgram(openGLid));
}

void Shader::UnBind()
{
    glCall(glUseProgram(0));
}

void Shader::SetUniform(const std::string& name, std::any value)
{
    std::string type = uniformTypesByName[name];
    if (type == "int")
        SetUniformInt(name, std::any_cast<int>(value));
    else if (type == "vec4")
        SetUniformVec4(name, std::any_cast<glm::vec4>(value));
    else if (type == "mat4")
        SetUniformMat4(name, std::any_cast<glm::mat4>(value));
    else if (type == "sampler2D")
        SetUniformTexture(name);
    else
    {
        type = type == "" ? "blank" : type;
        RaiseError(
            "Unrecognized type " + type + " for uniform " + name + "\n"
            "Consider creating an additional SetUniformXXX function to deal with it.");
    }
}

std::string Shader::to_string() const
{
    std::string newline = "\n    ";
    std::string out = "Shader:" + newline;

    std::string locationByNameStr = "{ ";
    for (const auto& pair : locationByName)
        locationByNameStr += pair.first + ": " + std::to_string(pair.second) + ", ";
    locationByNameStr += "}";

    std::string idByFilePathStr = "{ ";
    for (const auto& pair : idByFilePath)
        idByFilePathStr += pair.first + ": " + std::to_string(pair.second) + ", ";
    idByFilePathStr += "}";

    std::string uniformTypesByNameStr = "{ ";
    for (const auto& pair : uniformTypesByName)
        uniformTypesByNameStr += pair.first + ": " + pair.second + ", ";
    uniformTypesByNameStr += "}";

    std::string textureSlotsByNameStr = "{ ";
    for (const auto& pair : textureSlotsByName)
        textureSlotsByNameStr += pair.first + ": " + std::to_string(pair.second) + ", ";
    textureSlotsByNameStr += "}";

    int boundOpenGLid;
    glCall(glGetIntegerv(GL_CURRENT_PROGRAM, &boundOpenGLid));

    out += "id: "                 + logger::to_string(id)       + newline;
    out += "openGLid: "           + std::to_string(openGLid)    + newline;
    out += "locationByName: "     + locationByNameStr           + newline;
    out += "idByFilePath: "       + idByFilePathStr             + newline;
    out += "uniformTypesByName: " + uniformTypesByNameStr       + newline;
    out += "textureSlotsByName: " + textureSlotsByNameStr       + newline;
    out += "isBound: " + std::to_string(openGLid == boundOpenGLid) + newline;

    return out;
}

// ---------------- private ----------------

// ---------- internal uniform functions ----------
void Shader::SetUniformInt(const std::string& name, int i)
{
    glCall(glUniform1i(GetUniformLocation(name), i));
}
void Shader::SetUniformVec4(const std::string& name, const glm::vec4& vector4)
{
    glCall(glUniform4f(GetUniformLocation(name), vector4.r, vector4.g, vector4.b, vector4.a));
}
void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    auto pointerToStartOfMatrix = &matrix[0][0];
    glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, pointerToStartOfMatrix));
}
void Shader::SetUniformTexture(const std::string& name)
{
    glCall(glUniform1i(GetUniformLocation(name), textureSlotsByName[name]));
}



int Shader::GetUniformLocation(const std::string& name)
{
    if (locationByName.find(name) != locationByName.end())
        return locationByName[name];

    glCall(int location = glGetUniformLocation(openGLid, name.c_str()));
    if (location == -1)
        Warning("glGetUniformLocation failed to find uniform " + name + ".\n"
            + "Remember the shader language is case sensitive and that unused variables won't be found.");
    locationByName[name] = location;
    return location;
}


// ---------- parsing ----------

void Shader::CheckFilePath(const std::string& filePath)
{
    bool fileExists = std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    Assert(fileExists,
        "There is no shader file (nor any other file) at " + filePath);
    auto split = Tools::SplitString(filePath, ".");
    bool isShaderFile = split[split.size() - 1] == "shader";
    Assert(isShaderFile,
        "The shader filePath doesnt have the expected .shader extension " + filePath);
}

void Shader::FindUniforms(const std::string& filePath)
{
    using namespace std;
    ifstream stream(filePath);
    string line;

    while (getline(stream, line))
    {
        line = Tools::Trim(line); // remove spaces and newlines
        line = Tools::SplitString(line, "//")[0]; // remove single-line comments

        if (line.find("uniform") != string::npos)
        {
            // Parse uniform name from the line (simplified, assumes specific format)
            string uniform = line.substr(line.find(" ") + 1, line.find(";") - line.find(" ") - 1);
            vector<string> typeAndName = Tools::SplitString(uniform, " ");
            Assert(typeAndName.size() == 2,                
                "Expected typeAndName of uniform to have exactly two elements.\n"
                "uniform = " + uniform);
            uniformTypesByName[typeAndName[1]] = typeAndName[0];
            if (typeAndName[1].find("sampler2D") != string::npos) // what about 1D and 3D ?? evt. drop dimensionality suffix
                textureSlotsByName[typeAndName[0]] = textureSlotsByName.size(); // this counts up like an ID: 0,1,2...
        }
    }
}

ShaderStrings Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);
    std::string line;
    std::stringstream parsedShader[2];
    enum class ShaderType { none = -1, vertex = 0, fragment = 1 };
    ShaderType shaderType = ShaderType::none;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                shaderType = ShaderType::vertex;
            else if (line.find("fragment") != std::string::npos)
                shaderType = ShaderType::fragment;
            else
                RaiseError("Failed to parse shader. Unrecognized shader type.");
        }
        else if (shaderType != ShaderType::none)
        {
            parsedShader[(int)shaderType] << line << "\n";
        }
        else
            RaiseError("Failed to parse shader. Unrecognized shader type.");

    }

    return { parsedShader[0].str(), parsedShader[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    // compiling shader
    glCall(unsigned int subShaderID = glCreateShader(type));
    const char* source_ = source.c_str();
    glCall(glShaderSource(subShaderID, 1, &source_, nullptr));
    glCall(glCompileShader(subShaderID));

    // error handling
    int result;
    glCall(glGetShaderiv(subShaderID, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        glCall(glGetShaderiv(subShaderID, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)_malloca(length * sizeof(char)); // changed from alloca
        glCall(glGetShaderInfoLog(subShaderID, length, &length, message));
        std::string typeName = (type == GL_VERTEX_SHADER) ? "vertex" : "fragment";
        
        glCall(glDeleteShader(subShaderID));
        RaiseError("Failed to compile " + typeName + " shader\n" + message);
        return 0;
    }

    return subShaderID;
}

unsigned int Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    glCall(unsigned int shaderID = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glCall(glAttachShader(shaderID, vs));
    glCall(glAttachShader(shaderID, fs));
    glCall(glLinkProgram(shaderID));
    glCall(glValidateProgram(shaderID));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));
    return shaderID;
}








