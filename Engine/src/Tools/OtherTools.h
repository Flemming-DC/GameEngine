#pragma once
#include <any>



namespace Tools
{

    template <typename T>
    bool IsOfType(const std::any& variable)
    {
        try
        {
            T t = std::any_cast<T>(variable);
            return true;
        }
        catch (const std::bad_any_cast& _) 
        { 
            (void)_; // the expression (void)_ prevents an unused variable warning
            return false;
        } 
    }

    #pragma warning(push)
    #pragma warning(disable: 4172)
    template <typename T>
    T* TryAnyCast(const std::any& variable)
    {
        try
        {
            T t = std::any_cast<T>(variable);
            return &t;
        }
        catch (const std::bad_any_cast& _)
        {
            (void)_; // the expression (void)_ prevents an unused variable warning
            return nullptr;
        }
    }
    #pragma warning(pop) 
}







