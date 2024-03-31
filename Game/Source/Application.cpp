#include "Core.h"
#include "Dynamic.h"
#include "GameAssets.h"
#include "GameInputs.h"
#include "DemoScene.h"
#include "SecondScene.h"
#include "MiniScene.h"
#include "Settings.h"
#ifdef InEditor
    #include "EditorCore.h"
#endif // InEditor


int main()
{
    //logger::print(UuidCreator::MakeID());
    #ifdef InEditor
        EditorCore::MarkAsEditor();
    #endif // InEditor

    Dynamic::Setup<GameAssets, GameInputs, Settings>();
    Core::Run(std::make_unique<DemoScene>());
    return 0;
}

// findstr /S /M "_Debug" "Engine\Source\*.*"
// 
// git add . && git commit -m "m" && git push

