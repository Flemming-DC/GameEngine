#include "YmlTools.h"
#include "ErrorChecker.h"
#include "StringTools.h"
#include "EngineLiterals.h"
#include <fstream>
#include <filesystem>
#include <sstream>

namespace YmlTools
{
    void _EnsureYMLsuffix(std::string& path)
    {
        auto parts = Tools::SplitString(path, ".");
        bool hasExtension = parts.size() > 1;
        if (hasExtension && parts.back() == "yml")
            return;
        if (hasExtension && parts.back() == "yaml")
            return;
        if (hasExtension)
            RaiseError("The socalled yml file '" + path + "', should end on .yml or .yaml, not " + parts.back());
        path += ".yml";
    }

    void _CheckThatValidLocation(const std::string& path)
    {
        if (!Tools::StartsWith(path, Literals::Res))
            RaiseError("path is expected to start with '" + Literals::Res + "' received '" + path + "'");
    }


    YAML::Node Load(std::string path, bool createFileIfAbsent)
    {
        _CheckThatValidLocation(path);
        _EnsureYMLsuffix(path);
        if (!std::filesystem::exists(path))
        {
            if (createFileIfAbsent)
            {
                //if (!Tools::EndsWith(path, ".yml") && !Tools::EndsWith(path, ".yaml"))
                //    RaiseError("The socalled yml file " + path + ", should end on .yml or .yaml");
                std::ofstream file(path);
                file.close();
            }
            else
                RaiseError("Cannot load yml at '" + path + "', since there is no file there.");
        }
        return YAML::LoadFile(path);
    }

    void Save(YAML::Node yml, std::string path, bool overwrite, bool horizontalLists)
    {
        _CheckThatValidLocation(path);
        _EnsureYMLsuffix(path);
        if (!overwrite && std::filesystem::exists(path))
            RaiseError("Cannot save yml to '" + path + "', since there is already a file there.");

        // configure yaml file using emitter
        YAML::Emitter emitter;
        emitter.SetIndent(4);
        if (horizontalLists)
            emitter.SetSeqFormat(YAML::Flow); // write lists horizontally, not vertically
        emitter << yml;

        // write yaml data to output stream
        std::ofstream outStream(path);
        outStream << emitter.c_str();
        outStream.close();
    }
}
