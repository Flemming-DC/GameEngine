#include "Core.h"
#ifdef InEditor
    #include "EditorCore.h"
#endif // InEditor
#include "Dynamic.h"
#include "GameAssets.h"
#include "GameInputs.h"
#include "Settings.h"
#include "GameLiterals.h"
#include "DummyComp.h"
#include "GameLogic.h"
#include "RocketEngine.h"
#include "Entity.h"


int main()
{
#ifdef InEditor
    Editor::EditorCore::MarkAsEditor();
#endif // InEditor

    Entity::DeclareComps<DummyComp, GameLogic, RocketEngine>();
    Dynamic::Setup<GameAssets, Settings>();
    Core::Run(Literals::stroidalDevScene);
}

/*
logger::print(UuidCreator::MakeID());
findstr /S /M "_Debug" "Engine\Source\*.*"

git add . && git commit -m "m" && git push
*/


