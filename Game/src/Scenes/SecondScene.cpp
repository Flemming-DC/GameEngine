#include "SecondScene.h"
#include "Engine.h"
#include "GameLogic.h"
#include "GameAssets.h"
#include "EngineAssets.h"


void SecondScene::PurelyManualSetup()
{

    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();

    // ---------- cpp scene data ---------- 

    auto cameraID = Entity::GetID("camera");
    //auto cameraID = Entity::Make("camera");
    //Entity::Add<Transform>(cameraID);
    //Entity::Add<Camera>(cameraID).SetToOrthographic();

    auto picture1ID = Entity::Make("picture 1");
    Entity::Add<Transform>(picture1ID);
    Entity::Add<Renderable>(picture1ID).Setup(material);
    Entity::Add<RectangleCollider>(picture1ID).SetSize({ 1, 1 });


    auto picture2ID = Entity::Make("picture 2");
    Entity::Add<Transform>(picture2ID).SetParent(&Entity::Get<Transform>(picture1ID));
    Entity::Add<Renderable>(picture2ID).Setup(material);
    Entity::Add<RectangleCollider>(picture2ID).SetSize({ 1, 1 });
    Entity::Add<GameLogic>(picture2ID);



    // ---------- saved scene data ---------- 

    Entity::Get<Transform>(picture1ID).localPosition = { -0.5f, -0.5f, 0 };
    Entity::Get<Transform>(picture2ID).localPosition = { 1.5f, -0.5f, 0 };


    entityIDs = { cameraID, picture1ID, picture2ID };
    onStart.Invoke(*this);
}


void SecondScene::ManualSetup()
{
    // ------------ get assets ------------
    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();

    // ------------ get entities ------------
    auto picture1ID = Entity::GetID("picture 1");
    auto picture2ID = Entity::GetID("picture 2");

    // ------------ setup components ------------
    //Entity::Get<Renderable>(picture1ID).Setup(material);
    Entity::Get<Renderable>(picture2ID).Setup(material);
    Entity::Add<GameLogic>(picture2ID);

}


