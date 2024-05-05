#include "MiniScene.h"
#include "Engine.h"
#include "GameAssets.h"
#include "EngineAssets.h"
#include "DummyComp.h"


void MiniScene::PurelyManualSetup()
{

}


void MiniScene::OnStart()
{
    // ------------ get assets ------------
    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();

    // ------------ get entities ------------
    auto cameraID = Entity::GetID("camera");

    // ------------ setup components ------------
    //Entity::Add<DummyComp>(cameraID);

}


