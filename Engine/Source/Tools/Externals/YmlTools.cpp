#include "YmlTools.h"
#include "ErrorChecker.h"
#include <fstream>
#include <filesystem>
#include <sstream>

namespace YmlTools
{
    YAML::Node Load(std::string path)
    {
        if (!std::filesystem::exists(path))
            RaiseError("Cannot load yml at " + path + ", since there is no file there.");
        return YAML::LoadFile(path);
    }

    void Save(YAML::Node yml, std::string path, bool overwrite, bool horizontalLists)
    {
        if (!overwrite && std::filesystem::exists(path))
            RaiseError("Cannot save yml to " + path + ", since there is already a file there.");

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
