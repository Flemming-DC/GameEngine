#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <ListTools.h>

template <typename... Args>
class Event
{
public:
    unsigned int add(const std::function<void(Args...)>& function)
    {
        maxId++;
        functionsById[maxId] = function;
        return maxId;
    }

    void remove(unsigned int id)
    {
        Tools::RemoveKey_unordered(functionsById, id);
    }

    void invoke(Args... args) const
    {
        for (const auto& pair : functionsById)
            pair.second(args...);
    }

private:
    std::unordered_map<unsigned int, std::function<void(Args...)>> functionsById;
    std::atomic<unsigned int> maxId;
};
