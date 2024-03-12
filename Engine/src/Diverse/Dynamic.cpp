#include "Dynamic.h"
#include <ListTools.h>

std::vector<std::unique_ptr<Dynamic>> Dynamic::initializables;


bool Dynamic::Remove(Dynamic& initializable)
{
    return Tools::Remove(initializables,
        [&](const std::unique_ptr<Dynamic>& ptr) { return ptr.get() == &initializable; });
}

void Dynamic::Clear()
{
    initializables.clear();
}


void Dynamic::CallOnEngineStart()
{
    for (const auto& initializable : initializables)
    {
        if (initializable)
            initializable->OnEngineStart();
        else
            RaiseError(Tools::TypeName(initializable) + " is nullptr");
    }
}

void Dynamic::CallOnGameStart()
{
    for (const auto& initializable : initializables)
    {
        if (initializable)
            initializable->OnGameStart();
        else
            RaiseError(Tools::TypeName(initializable) + " is nullptr");
    }
}

void Dynamic::CallOnUpdate()
{
    for (const auto& initializable : initializables)
    {
        if (initializable)
            initializable->OnUpdate();
        else
            RaiseError(Tools::TypeName(initializable) + " is nullptr");
    }
}

void Dynamic::CallOnGameEnd()
{
    for (const auto& initializable : initializables)
    {
        if (initializable)
            initializable->OnGameEnd();
        else
            RaiseError(Tools::TypeName(initializable) + " is nullptr");
    }
}

void Dynamic::CallOnEngineEnd()
{
    for (const auto& initializable : initializables)
    {
        if (initializable)
            initializable->OnEngineEnd();
        else
            RaiseError(Tools::TypeName(initializable) + " is nullptr");
    }
}










