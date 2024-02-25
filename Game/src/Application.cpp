#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Initializer.h"
#include "Material.h"
#include "Renderable.h"
#include "Mesh.h"
#include <map>
#include <unordered_map>
#include <vector>
#include "Event.h"
#include "Engine.h"
#include "Components/GameLogic.h"
#include "CollisionLoop.h"
#include "CollisionChecker.h"
#include "Input.h"
#include "Register.h"
#include "UuidCreator.h"
#include "GameAssets.h"
#include "EngineAssets.h"
#include "DemoScene.h"
#include "ListTools.h"
#include "Editor.h"


void HelloEnter(Collider* other)
{
    Log("Enter " + other->GetEntity().name);
}
void HelloExit(Collider* other)
{
    Log("Exit " + other->GetEntity().name);
}




void run()
{
    Log(UuidCreator::to_string(UuidCreator::MakeID()));
    Initializer::Setup();
    Renderer::SetupGrid2D(0.25f);
    GameAssets::Setup();

    // ---------- assets ---------- 
    

    DemoScene demoScene;
    demoScene.Setup();
    //demoScene.Save();
    //demoScene.Load();



    // ------------ loop ------------
    

    Log("--------- starting loop --------- ");
    while (Initializer::NewFrame())
    {
        CollisionLoop::Update();
        Editor::Update();
        Entity::UpdateAllEntities();
        Renderer::Draw();
        Initializer::EndFrame();
    }
    Log("------- Shutdown --------");
    Initializer::Shutdown();
}


int main()
{
    run();
    return 0;
}
// git add . && git commit -m "m" && git push
