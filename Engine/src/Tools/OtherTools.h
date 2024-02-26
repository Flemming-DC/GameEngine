#pragma once
#include <any>



namespace Tools
{
    template <typename T>
    T* TryAnyCast(const std::any& variable)
    {
        if (variable.type() == typeid(T))
        {
            T t = std::any_cast<T>(variable);
            return &t;
        }
        else
            return nullptr;
    }
    
}







