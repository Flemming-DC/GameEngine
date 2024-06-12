#include "Dynamic.h"
#include <ListTools.h>
#include "Profiler.h"

std::vector<std::unique_ptr<Dynamic>> Dynamic::dynamics;


void Dynamic::ShutDown(Dynamic& dynamic)
{
    Tools::Remove(dynamics,
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
        if (!dynamic)
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
        dynamic->OnEditorStart();
    }
}

void Dynamic::CallOnGameStart()
{
    for (const auto& dynamic : dynamics)
    {
        if (!dynamic)
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
        dynamic->OnGameStart();
    }
}

void Dynamic::CallOnUpdate()
{
    for (const auto& dynamic : dynamics)
    {
        if (!dynamic)
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
        dynamic->OnUpdate();
    }
}

void Dynamic::CallOnEditorUpdate()
{
    ProfileFunc;
    for (const auto& dynamic : dynamics)
    {
        if (!dynamic)
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
        dynamic->OnEditorUpdate();
    }
}


void Dynamic::CallOnGameEnd()
{
    for (const auto& dynamic : dynamics)
    {
        if (!dynamic)
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
        dynamic->OnGameEnd();
    }
}

void Dynamic::CallOnEditorEnd()
{
    for (const auto& dynamic : dynamics)
    {
        if (!dynamic)
            RaiseError(Tools::TypeName(dynamic) + " is nullptr");
        dynamic->OnEditorEnd();
    }
}










