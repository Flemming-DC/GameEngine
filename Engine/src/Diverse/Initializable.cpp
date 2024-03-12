#include "Initializable.h"
#include <ListTools.h>

std::unordered_map<unsigned int, std::unique_ptr<Initializable>> Initializable::initializables;
unsigned int Initializable::maxId;

/*
unsigned int Initializable::Make()
{
    maxId++;
    initializables[maxId] = std::make_unique<Initializable>();
    return maxId;
}*/

bool Initializable::Remove(unsigned int id)
{
    // we do not need to do maxId--, since its not an index, but a key in a map
    return Tools::RemoveKey_unordered(initializables, id);
}

void Initializable::Clear()
{
    initializables.clear();
}

void Initializable::CallOnEngineStart()
{
    for (const auto& [id, initializable] : initializables)
        initializable->OnEngineStart();
}

void Initializable::CallOnGameStart()
{
    for (const auto& [id, initializable] : initializables)
        initializable->OnGameStart();
}
/*
void Initializable::CallOnSceneStart()
{

}

void Initializable::CallOnSceneEnd()
{

}
*/
void Initializable::CallOnGameEnd()
{
    for (const auto& [id, initializable] : initializables)
        initializable->OnGameEnd();
}

void Initializable::CallOnEngineEnd()
{
    for (const auto& [id, initializable] : initializables)
        initializable->OnEngineEnd();
}










