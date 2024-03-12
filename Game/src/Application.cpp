#include "Renderer.h"
#include "Initializer.h"
#include "CollisionLoop.h"
#include "GameAssets.h"
#include "DemoScene.h"
#include "SecondScene.h"
#include "MiniScene.h"
#include "Entity.h"
#include "Renderable.h"
#include "Editor.h" // evt. temp
#include "FrameBuffer.h"
#include "Input.h" // temp
#include "Dynamic.h"

void run()
{
    //logger::print(UuidCreator::MakeID());
    Initializer::Setup();
    Dynamic::CallOnEngineStart();
    Renderer::ShowWindow(false); // if is_editor
    GameAssets::Setup(); // not a part of the engine
    Dynamic::CallOnGameStart();

    Scene::Activate<SecondScene>();
    //Scene::Activate<DemoScene>();

    //Scene::MakeBlankSceneFile("SecondScene");
    //Scene::GetActiveScene().PurelyManualSetup();
    //Scene::GetActiveScene().Save();
    
    
    Log("--------- starting loop --------- ");
    while (Initializer::NewFrame())
    {
        CollisionLoop::Update();
        Editor::Update(); // if is_editor
        Dynamic::CallOnUpdate();
        Entity::UpdateAllEntities();
        Renderer::DrawToScreen();
        Delay::OnFrameEnd();
        Initializer::EndFrame();
    }
    Log("------- Shutdown --------");
    Scene::GetActiveScene().ShutDown();
    Dynamic::CallOnGameEnd();
    Dynamic::CallOnEngineEnd();
    Initializer::Shutdown(); 
    Log("------- Done --------");
}


int main()
{
    run();
    return 0;
}
// git add . && git commit -m "m" && git push
