#include "MiniScene.h"
#include "Engine.h"
#include "GameAssets.h"
#include "EngineAssets.h"
#include "Components/DummyComp.h"


void MiniScene::PurelyManualSetup()
{

}


void MiniScene::ManualSetup()
{
    // ------------ get assets ------------
    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();

    // ------------ get entities ------------
    auto cameraID = Entity::GetID("camera");

    // ------------ setup components ------------
    Entity::Add<DummyComp>(cameraID);

}


