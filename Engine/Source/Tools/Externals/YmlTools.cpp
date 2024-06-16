#include "YmlTools.h"
#include "ErrorChecker.h"
#include "StringTools.h"
#include "EngineLiterals.h"
#include <fstream>
#include <filesystem>
#include <sstream>

Shorts;

namespace YmlTools
{
    void _EnsureYMLsuffix(string& path)
    {
        auto parts = Tools::SplitString(path, ".");
        bool hasExtension = parts.size() > 1;
        if (hasExtension && parts.back() == "yml")
            return;
        if (hasExtension && parts.back() == "yaml")
            return;
        Deny(hasExtension,
            "The socalled yml file '" + path + "', should end on .yml or .yaml, not " + parts.back());
        path += ".yml";
    }

    void _CheckValidLocation(const string& path)
    {
        Assert(Tools::StartsWith(path, Literals::Res),
            "path is expected to start with '" + Literals::Res + "' received '" + path + "'");
    }


    Node Load(string path, bool createFileIfAbsent)
    {
        _CheckValidLocation(path);
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

    void Save(Node yml, string path, bool overwrite, bool horizontalLists)
    {
        _CheckValidLocation(path);
        _EnsureYMLsuffix(path);
        Deny(!overwrite && std::filesystem::exists(path),
            "Cannot save yml to '" + path + "', since there is already a file there.");

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

    // equality test


    bool _AreEqualSequences(Node n1, Node n2)
    {
        if (n1.size() != n2.size())
            return false;
        for (int i = 0; i < (int)n1.size(); i++)
        {
            if (!IsEqual(n1[i], n2[i]))
                return false;
        }
        return true;
    }

    bool _AreEqualMaps(Node n1, Node n2)
    {
        if (n1.size() != n2.size())
            return false;

        // check if keys are different
        for (auto& pair2 : n2)
        {
            bool key2IsInKeys1 = false;
            for (auto& pair1 : n1)
            {
                if (IsEqual(pair2.first, pair1.first))
                {
                    key2IsInKeys1 = true;
                    break;
                }
            }
            if (!key2IsInKeys1)
                return false;
        }

        vector<Node*> keys;
        for (auto& pair : n1)
            keys.push_back(&pair.first);

        for (Node* k : keys)
        {
            if (!IsEqual(n1[*k], n2[*k]))
                return false;
        }

        return true;
    }

    bool IsEqual(Node n1, Node n2)
    {
        if (n1.Type() != n2.Type())
            return false;
        auto type = n1.Type();
        
        switch (type)
        {
        case YAML::NodeType::Undefined:
            return true;
        case YAML::NodeType::Null:
            return true;
        case YAML::NodeType::Scalar:
            return n1.Scalar() == n2.Scalar();
        case YAML::NodeType::Sequence:
            return _AreEqualSequences(n1, n2);
        case YAML::NodeType::Map:
            return _AreEqualMaps(n1, n2);
        default:
            RaiseError("UnRecognized nodeType ", type, " for node:\n", n1);
        }

    }

}
