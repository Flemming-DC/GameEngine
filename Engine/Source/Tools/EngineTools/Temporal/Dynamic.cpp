#include "Dynamic.h"
#include <ListTools.h>

std::vector<std::unique_ptr<Dynamic>> Dynamic::dynamics;


bool Dynamic::Remove(Dynamic& dynamic)
{
    return Tools::Remove(dynamics,
        [&](const std::unique_ptr<Dynamic>& ptr) { return ptr.get() == &dynamic; });
}

void Dynamic::Clear()
{
    dynamics.clear();
}


void Dynamic::CallOnEditorStart()
{
    for (const auto& dynamic : dynamics)
    {
        if (dynamic)
            dynamic->OnEditorStart();
        else
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
    }
}

void Dynamic::CallOnGameStart()
{
    for (const auto& dynamic : dynamics)
    {
        if (dynamic)
            dynamic->OnGameStart();
        else
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
    }
}

void Dynamic::CallOnUpdate()
{
    for (const auto& dynamic : dynamics)
    {
        if (dynamic)
            dynamic->OnUpdate();
        else
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
    }
}

void Dynamic::CallOnGameEnd()
{
    for (const auto& dynamic : dynamics)
    {
        if (dynamic)
            dynamic->OnGameEnd();
        else
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
    }
}

void Dynamic::CallOnEditorEnd()
{
    for (const auto& dynamic : dynamics)
    {
        if (dynamic)
            dynamic->OnEditorEnd();
        else
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
    }
}










