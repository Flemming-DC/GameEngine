#include "DemoScene.h"
#include "Engine.h"
#include "Components/GameLogic.h"
#include "GameAssets.h"
#include "EngineAssets.h"

void DemoScene::HelloEnter(Collider* other)
{
    Log("DemoScene::Enter " + other->GetEntity().name);
}
void DemoScene::HelloExit(Collider* other)
{
    Log("DemoScene::Exit " + other->GetEntity().name);
}
/*
DemoScene::DemoScene(const std::string& name) : Scene(name)
{
    entities = MakeEntities();
    LoadSpatialData(entities);
}*/

std::vector<uuids::uuid>& DemoScene::MakeEntities()
{

    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();
    //std::vector<Entity> _entities;

    // ---------- cpp scene data ---------- 

    auto cameraID = Entity::Make("camera");
    Entity::Add<Transform>(cameraID);
    Entity::Add<Camera>(cameraID).SetOrthographicProjection();

    auto picture1ID = Entity::Make("picture 1");
    Entity::Add<Transform>(picture1ID);
    Entity::Add<Renderable>(picture1ID).SetByInspector(mesh, material);
    Entity::Add<RectangleCollider>(picture1ID).SetSize({ 1, 1 });


    auto picture2ID = Entity::Make("picture 2");
    Entity::Add<Transform>(picture2ID).SetParent(&Entity::Get<Transform>(picture1ID));
    Entity::Add<Renderable>(picture2ID).SetByInspector(mesh, material);
    Entity::Add<GameLogic>(picture2ID).SetByInspector(material);
    Entity::Add<RectangleCollider>(picture2ID).SetSize({ 1, 1 });
    Entity::Get<RectangleCollider>(picture2ID).onEnter.Add(HelloEnter);
    Entity::Get<RectangleCollider>(picture2ID).onExit.Add(HelloExit);

    auto circle1ID = Entity::Make("circle 1");
    Entity::Add<Transform>(circle1ID);
    Entity::Add<CircleCollider>(circle1ID).SetLocalRadius(0.5f);

    auto circle2ID = Entity::Make("circle 2");
    Entity::Add<Transform>(circle2ID);
    Entity::Add<CircleCollider>(circle2ID).SetLocalRadius(0.5f);
    Entity::Get<CircleCollider>(circle2ID).onEnter.Add(HelloEnter);
    Entity::Get<CircleCollider>(circle2ID).onExit.Add(HelloExit);


    // ---------- saved scene data ---------- 

    Entity::Get<Transform>(picture1ID).localPosition = { -0.5f, -0.5f, 0 };
    Entity::Get<Transform>(picture2ID).localPosition = { 1.5f, -0.5f, 0 };
    Entity::Get<Transform>(circle1ID ).localPosition = { -0.5f,  0.6f, 0 };
    Entity::Get<Transform>(circle2ID ).localPosition = { 0.7f,  0.5f, 0 };

    

    entities.push_back(picture1ID);
    entities.push_back(picture2ID);
    entities.push_back(circle1ID);
    entities.push_back(circle2ID);

    return entities;
}





