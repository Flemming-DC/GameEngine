#include "GameLogic.h"
#include "InputKey.h"
#include "ColQuery.h"
#include "Renderer.h"
#include "DemoScene.h"
#include "SecondScene.h"
#include "Time_.h"
#include "InputVectorizer.h"

using namespace Key;
Shorts;

void GameLogic::OnStart()
{
    material = &Get<Renderable>().GetMaterial();
    color = material->GetUniform<glm::vec4>("u_color");
    /*
    onEnterIndex = Get<RectangleCollider>().onEnter.Add([this](Collider& other)
        { 
            logger::print("GameLogic.OnStart: Collider.Enter: " + other.Entity().Name() + " enters " + this->Entity().Name());
        });
    onExitIndex = Get<RectangleCollider>().onExit.Add([this](Collider& other)
        { 
            logger::print("GameLogic: Collider.onExit: " + other.Entity().Name() + " enters " + this->Entity().Name()); 
        });
    */
}

void GameLogic::OnDestroy() 
{ 
    /*
    Get<RectangleCollider>().onEnter.Remove(onEnterIndex);
    Get<RectangleCollider>().onExit.Remove(onExitIndex);
    */

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
    //CollisionTests();
    //CollisionTests();
}



void GameLogic::CollisionTests()
{
    if (InputKey::BecomesPressed(Keyboard::P))
    {
        logger::print("P");
        auto overlaps = ColQuery::Overlaps(ColMaker::Circle(vec2(0.5f), 0.49f));
        for (const auto& overlap : overlaps)
            logger::print(overlap->Entity().Name() + " overlaps Circle(vec2(0.49f), 0.49f)");
    }
    if (InputKey::BecomesPressed(Keyboard::O))
    {
        logger::print("O");
        auto overlaps = ColQuery::Overlaps(ColMaker::Point(vec2(-0.2f)));
        for (const auto& overlap : overlaps)
            logger::print(overlap->Entity().Name() + " overlaps Point(vec2(-0.2f))");
    }
    if (InputKey::BecomesPressed(Keyboard::I))
    {
        logger::print("I");
        auto overlaps = ColQuery::Overlaps(ColMaker::Polygon({ vec2(0.5f), vec2(-0.5f), vec2(-0.5f, 0.5f) }));
        for (const auto& overlap : overlaps)
            logger::print(overlap->Entity().Name() + " overlaps Polygon({ vec2(0.5f), vec2(-0.5f), vec2(-0.5f, 0.5f) })");
    }
    if (InputKey::BecomesPressed(Keyboard::U))
    {
        logger::print("U");
        auto overlaps = ColQuery::Overlaps(ColMaker::Rectangle(vec2(0.5f), quat(), vec2(2.0f)));
        for (const auto& overlap : overlaps)
            logger::print(overlap->Entity().Name() + " overlaps Rectangle(vec2(0.5f), quat(), vec2(2.0f)");
    }
    if (InputKey::BecomesPressed(Keyboard::Y))
    {
        logger::print("Y");
        auto collider = ColQuery::TryGetOverlap(ColMaker::Circle(vec2(0.5f), 0.49f));
        logger::print("TryGetOverlap with Circle(vec2(0.5f), 0.5f) yields ", collider);
    }
    if (InputKey::BecomesPressed(Keyboard::T))
    {
        logger::print("T");
        bool isOverlapping = ColQuery::IsOverlapping(ColMaker::Circle(vec2(0.5f), 0.51f), ColMaker::Circle(vec2(-0.5f), 0.51f));
        logger::print("isOverlapping = ", isOverlapping, " distance = ", glm::length((vec2(0.5f) - vec2(-0.5f))), " r1+r2 = ", 0.51f + 0.51f);
    }


}

void GameLogic::OldCollisionTests()
{

    if (InputKey::BecomesPressed(Keyboard::P))
    {
        logger::print("P");
        for (const auto& overlap : ColQuery::AllOverlaps())
            logger::print(overlap.first->Entity().Name() + " overlaps " + overlap.second->Entity().Name());
    }
    if (InputKey::BecomesPressed(Keyboard::O))
    {
        logger::print("O");
        auto colliders = ColQuery::RayOverlaps(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
        for (const auto& col : colliders)
            logger::print(col->Entity().Name() + " was hit");
    }
    if (InputKey::BecomesPressed(Keyboard::I))
    {
        logger::print("I");
        auto collider = ColQuery::RayCast(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
        if (collider)
            logger::print(collider->Entity().Name() + " was hit");
        else
            logger::print("Hit nothing");
    }

}






