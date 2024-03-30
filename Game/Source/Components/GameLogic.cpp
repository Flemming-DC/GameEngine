#include "GameLogic.h"
#include "InputKey.h"
#include "CollisionChecker.h"
#include "Renderer.h"
#include "DemoScene.h"
#include "SecondScene.h"
#include "Time_.h"

using namespace Key;

void GameLogic::OnStart()
{
    material = &Get<Renderable>().GetMaterial();
    color = material->GetUniform<glm::vec4>("u_color");

    onEnterIndex = Get<RectangleCollider>().onEnter.Add([this](Collider& other)
        { 
            logger::print("GameLogic.OnStart: Collider.Enter: " + other.GetEntity().name + " enters " + this->GetEntity().name);
        });
    onExitIndex = Get<RectangleCollider>().onExit.Add([this](Collider& other)
        { 
            logger::print("GameLogic: Collider.Enter: " + other.GetEntity().name + " enters " + this->GetEntity().name); 
        });
}

void GameLogic::OnDestroy() 
{ 
    Get<RectangleCollider>().onEnter.Remove(onEnterIndex);
    Get<RectangleCollider>().onExit.Remove(onExitIndex);


}

void GameLogic::OnUpdate()
{
    material->SetUniform("u_color", color);

    if (color.r > 1)
        increment = -6.0f * Time::Delta();
    if (color.r < 0)
        increment = 6.0f * Time::Delta();
    color.r += increment;



    if (InputKey::BecomesPressed(Keyboard::M))
    {
        logger::print("M");
        Scene::Activate<DemoScene>();
        //Scene::Activate(new DemoScene());
    }
    if (InputKey::BecomesPressed(Keyboard::N))
    {
        logger::print("N");
        Scene::Activate<SecondScene>();
        //Scene::Activate(new SecondScene());
    }

}

void GameLogic::OldTests()
{
    material->SetUniform("u_color", color);
    
    if (color.r > 1)
        increment = -6.0f * Time::Delta();
    if (color.r < 0)
        increment = 6.0f * Time::Delta();
    color.r += increment;


    if (InputKey::BecomesPressed(Keyboard::K))
    {
        logger::print("K");
        for (const auto& overlap : CollisionChecker::GetOverlaps())
            logger::print(overlap.first->GetEntity().name + " overlaps " + overlap.second->GetEntity().name);
    }
    if (InputKey::BecomesPressed(Keyboard::L))
    {
        logger::print("L");
        auto colliders = CollisionChecker::RayOverlaps(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
        for (const auto& col : colliders)
            logger::print(col->GetEntity().name + " was hit");
    }
    if (InputKey::BecomesPressed(Keyboard::P))
    {
        logger::print("P");
        auto collider = CollisionChecker::RayCast(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
        if (collider)
            logger::print(collider->GetEntity().name + " was hit");
        else
            logger::print("Hit nothing");
    }

    if (InputKey::BecomesPressed(Keyboard::O))
    {
        logger::print("O");
        Renderer::ShowWindow(!Renderer::IsWindowVisible());
    }
    if (InputKey::BecomesPressed(Keyboard::M))
    {
        logger::print("M");
        Scene::Activate<DemoScene>(); // this gets called multiple frames in a row, which you shouldn't do in a real game
    }
    if (InputKey::BecomesPressed(Keyboard::N))
    {
        logger::print("N");
        Scene::Activate<SecondScene>(); // this gets called multiple frames in a row, which you shouldn't do in a real game
    }
}






