#include "DemoScene.h"
#include "Engine.h"
#include "GameAssets.h"
#include "EngineAssets.h"
#include "DummyComp.h"
#include "GameLogic.h"


void DemoScene::PurelyManualSetup()
{

    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();

    // ---------- cpp scene data ---------- 

    auto cameraID = Entity::register_.Add("camera").GetID();
    Entity::Add<Transform>(cameraID);
    Entity::Add<Camera>(cameraID).SetToOrthographic();

    auto picture1ID = Entity::register_.Add("picture 1").GetID();
    Entity::Add<Transform>(picture1ID);
    Entity::Add<Renderable>(picture1ID).Setup(material);
    Entity::Add<RectangleCollider>(picture1ID).SetSize({ 1, 1 });


    auto picture2ID = Entity::register_.Add("picture 2").GetID();
    Entity::Add<Transform>(picture2ID).SetParent(&Entity::Get<Transform>(picture1ID));
    Entity::Add<Renderable>(picture2ID).Setup(material);
    Entity::Add<GameLogic>(picture2ID);
    //Entity::Add<RectangleCollider>(picture2ID).SetSize({ 1, 1 });

    auto circle1ID = Entity::register_.Add("circle 1").GetID();
    Entity::Add<Transform>(circle1ID);
    Entity::Add<CircleCollider>(circle1ID).Setup(0.5f);

    auto circle2ID = Entity::register_.Add("circle 2").GetID();
    Entity::Add<Transform>(circle2ID);
    Entity::Add<CircleCollider>(circle2ID).Setup(0.5f);


    // ---------- saved scene data ---------- 

    Entity::Get<Transform>(picture1ID).localPosition = { -0.5f, -0.5f, 0 };
    Entity::Get<Transform>(picture2ID).localPosition = { 1.5f, -0.5f, 0 };
    Entity::Get<Transform>(circle1ID ).localPosition = { -0.5f,  0.6f, 0 };
    Entity::Get<Transform>(circle2ID ).localPosition = { 0.7f,  0.5f, 0 };

    
    //entityIDs = { cameraID, picture1ID, picture2ID, circle1ID, circle2ID };
    onStart.Invoke(*this);
}


void DemoScene::ManualSetup()
{
    // ------------ get assets ------------
    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();
    
    // ------------ get entities ------------
    Entity::GetID("circle 1");
    Entity& picture1 = Entity::GetEntity("picture 1");
    Entity& picture2 = Entity::GetEntity("picture 2");

    // ------------ setup components ------------
    picture1.Get<Renderable>().Setup(material);
    picture2.Get<Renderable>().Setup(material);
    //picture2.Add<GameLogic>();
    //picture2.Add<DummyComp>();

    picture2.AddMultiple<GameLogic, DummyComp>();

    /*
    // ------------ get entities ------------
    Entity::GetID("circle 1");
    auto picture1ID = Entity::GetID("picture 1");
    auto picture2ID = Entity::GetID("picture 2");

    // ------------ setup components ------------
    Entity::Get<Renderable>(picture1ID).Setup(material);
    Entity::Get<Renderable>(picture2ID).Setup(material);
    Entity::Add<GameLogic>(picture2ID);
    Entity::Add<DummyComp>(picture2ID);
    */
}


