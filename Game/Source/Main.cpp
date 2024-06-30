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
#include "AsteroidSpawner.h"
#include "Impact.h"
#include "Player.h"
#include "AsteroidMotion.h"
#include "FollowPlayer.h"


int main()
{

#ifdef InEditor
    Editor::EditorCore::MarkAsEditor();
#endif // InEditor


    Entity::DeclareComps<DummyComp, GameLogic, RocketEngine, Impact, Player, AsteroidMotion, FollowPlayer>();
    Dynamic::Setup<GameAssets, Settings, AsteroidSpawner>();
    Core::Run(Literals::stroidalDevScene);
}

/*
logger::print(UuidCreator::MakeID());
findstr /S /M "_Debug" "Engine\Source\*.*"

git add . && git commit -m "m" && git push
*/


