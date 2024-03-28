#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <ListTools.h>

template <typename... Args>
class Event
{
public:
    unsigned int Add(const std::function<void(Args...)>& function)
    {
        maxId++;
        functionsById[maxId] = function;
        return maxId;
    }

    bool Remove(unsigned int id)
    {
        // we do not need to do maxId--, since its not an index, but a key in a map
        return Tools::RemoveKey(functionsById, id);
    }

    void Clear()
    {
        functionsById.clear();
    }

    void Invoke(Args... args) const
    {
        for (const auto& pair : functionsById)
            pair.second(args...);
    }

private:
    std::unordered_map<unsigned int, std::function<void(Args...)>> functionsById;
    unsigned int maxId;
};
