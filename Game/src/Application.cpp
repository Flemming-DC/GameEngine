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

void run()
{
    //Log(UuidCreator::to_string(UuidCreator::MakeID()));

    Initializer::Setup();
    Renderer::ShowWindow(false); // if is_editor
    GameAssets::Setup(); // not a part of the engine


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
        Entity::UpdateAllEntities();
        Renderer::DrawToScreen();
        Delay::OnFrameEnd();
        Initializer::EndFrame();
    }
    Log("------- Shutdown --------");
    Initializer::Shutdown();
    Log("------- Done --------");
}


int main()
{
    run();
    return 0;
}
// git add . && git commit -m "m" && git push
