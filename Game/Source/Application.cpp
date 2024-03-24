#include "Core.h"
#include "Dynamic.h"
#include "GameAssets.h"
#include "Editor.h" 
#include "DemoScene.h"
#include "SecondScene.h"
#include "MiniScene.h"
#include "ShortHands.h"


//Scene::MakeBlankSceneFile("SecondScene");
//Scene::GetActiveScene().PurelyManualSetup();
//Scene::GetActiveScene().Save();



int main()
{
    //logger::print(UuidCreator::MakeID());
    Dynamic::Setup<Editor, GameAssets>();
    Core::Run<DemoScene>();
    return 0;
}
// findstr /S /M "_Debug" "Engine\Source\*.*"
// 
// git add . && git commit -m "m" && git push

