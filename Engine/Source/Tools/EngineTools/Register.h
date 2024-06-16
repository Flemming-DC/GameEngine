#pragma once
#include <stduuid/uuid.h>
#include <utility>
#include "ErrorChecker.h"
#include "UuidCreator.h"
#include "ListTools.h"
#include "StringTools.h"

// the the use of uninitialized ID doesn't line up with the register, 
// so the register only allows initiliazed values. 
template<typename DataChunk>
class Register
{
public:

    template<typename... Args>
    DataChunk& Add(Args&&... args)
    {
        dataChunks.emplace_back(std::forward<Args>(args)...);
        indexByID[dataChunks.back().GetID()] = dataChunks.size() - 1;
        return dataChunks.back();
    }


    void Remove(const uuids::uuid& id) // this has not passed testing
    {
        Assert(Tools::ContainsKey(indexByID, id),
            "Trying to remove an object, which isn't in the register.");
        unsigned int index = indexByID.at(id);
        for (const auto& [id_, index_] : indexByID)
        {
            if (index_ > index)
                indexByID.at(id_) = index_ - 1;
        }
        Tools::RemoveKey(indexByID, id);
        dataChunks.erase(dataChunks.begin() + index);
    }

    DataChunk& Get(const uuids::uuid& id) { return dataChunks[indexByID.at(id)]; }
    std::vector<DataChunk>& GetData() { return dataChunks; }
    bool Contains(const uuids::uuid& id) const { return Tools::ContainsKey(indexByID, id); }

    void Clear()
    {
        dataChunks.clear();
        indexByID.clear();
    }

private:
    std::vector<DataChunk> dataChunks;
    std::unordered_map<uuids::uuid, unsigned int> indexByID;


};

