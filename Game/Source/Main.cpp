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
    #ifdef InEditor
        Editor::EditorCore::MarkAsEditor();
    #endif // InEditor

    Dynamic::Setup<GameAssets, GameInputs, Settings>();
    Core::Run(std::make_unique<DemoScene>());
}


// logger::print(UuidCreator::MakeID());
// findstr /S /M "_Debug" "Engine\Source\*.*"
// 
// git add . && git commit -m "m" && git push

