#include "Initializable.h"
#include <ListTools.h>

std::vector<std::unique_ptr<Initializable>> Initializable::initializables;


bool Initializable::Remove(Initializable& initializable)
{
    return Tools::Remove(initializables,
        [&](const std::unique_ptr<Initializable>& ptr) { return ptr.get() == &initializable; });
}

void Initializable::Clear()
{
    initializables.clear();
}

void Initializable::CallOnEngineStart()
{
    for (const auto& initializable : initializables)
        initializable->OnEngineStart();
}

void Initializable::CallOnGameStart()
{
    for (const auto& initializable : initializables)
        initializable->OnGameStart();
}

void Initializable::CallOnGameEnd()
{
    for (const auto& initializable : initializables)
        initializable->OnGameEnd();
}

void Initializable::CallOnEngineEnd()
{
    for (const auto& initializable : initializables)
        initializable->OnEngineEnd();
}










