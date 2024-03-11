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
#include "logger.h" // temp
#include "logger.h"

void run()
{
    //Log(log::to_string("hi"));
    
    Log(logger::to_string(3));
    Log(logger::to_string(false));
    Log(logger::make_string(3, ", ", false));
    logger::print(3, ", ", false);
    p(3, ", ", false, ", ", glm::vec3(1, 3, 2));

    bool b = true;
    p(&b, ", ", UuidCreator::MakeID());

    std::map<int, bool> m = {
        {1, true},
        {5, false},
        {2, true}
    };
    p(m);
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
