#include "GameLogic.h"
#include "Input.h"
#include "CollisionChecker.h"


void GameLogic::OnStart()
{
    material = &Get<Renderable>().GetMaterial();
    color = material->GetUniform<glm::vec4>("u_color");

    Get<RectangleCollider>().onEnter.Add([](Collider& other)
        { Log("DemoScene::Enter " + other.GetEntity().name); });
    Get<RectangleCollider>().onExit.Add([](Collider& other)
        { Log("DemoScene::Exit " + other.GetEntity().name); });
}

void GameLogic::OnUpdate()
{
    material->SetUniform("u_color", color);
    
    if (color.r > 1)
        increment = -0.1f;
    if (color.r < 0)
        increment = 0.1f;
    color.r += increment;


    if (Input::KeyHeldDown(Keyboard::K))
    {
        Log("K");
        for (const auto& overlap : CollisionChecker::GetOverlaps())
            Log(overlap.first->GetEntity().name + " overlaps " + overlap.second->GetEntity().name);
    }
    if (Input::KeyHeldDown(Keyboard::L))
    {
        Log("L");
        auto colliders = CollisionChecker::RayOverlaps(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
        for (const auto& col : colliders)
            Log(col->GetEntity().name + " was hit");
    }
    if (Input::KeyHeldDown(Keyboard::P))
    {
        Log("P");
        auto collider = CollisionChecker::RayCast(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
        if (collider != nullptr)
            Log(collider->GetEntity().name + " was hit");
        else
            Log("Hit nothing");
    }


}






