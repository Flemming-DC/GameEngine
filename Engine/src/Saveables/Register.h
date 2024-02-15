#pragma once
#include <stduuid/uuid.h>
#include <utility>
#include "ErrorChecker.h"
#include "UuidCreator.h"


template<typename DataChunk>
class Register
{
public:
    Register() { Log("Register constructed"); }

    // this the use of uninitialized ID doesn't line up with the register, 
    // so the register only allows initiliazed values. 
    template<typename... Args>
    DataChunk& Add(Args&&... args)
    {
        dataChunks.emplace_back(std::forward<Args>(args)...);
        indexByID[dataChunks.back().GetID()] = dataChunks.size() - 1;
        return dataChunks.back();
    }


    void Remove(const uuids::uuid& id)
    {
        if (!Tools::ContainsKey_unordered(indexByID, id))
            RaiseError("Trying to reomve an object, which isn't in the register.");
        dataChunks.erase(dataChunks.begin() + indexByID[id]);
        Tools::RemoveKey_unordered(indexByID, id);
    }

    const DataChunk& Get(const uuids::uuid& id)
    {
        return dataChunks[indexByID[id]];
    }


private:
    std::vector<DataChunk> dataChunks;
    std::unordered_map<uuids::uuid, int> indexByID;


};

