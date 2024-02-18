#include "DemoScene.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
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
    Entity::Add<Camera>(cameraID)->SetOrthographicProjection();

    auto picture1ID = Entity::Make("picture 1");
    Entity::Add<Transform>(picture1ID);
    Entity::Add<Renderable>(picture1ID)->SetByInspector(mesh, material);
    Entity::Add<RectangleCollider>(picture1ID)->SetSize({ 1, 1 });

    auto picture2ID = Entity::Make("picture 2");
    Entity::Add<Transform>(picture2ID)->SetParent(Entity::Get<Transform>(picture1ID));
    Entity::Add<Renderable>(picture2ID)->SetByInspector(mesh, material);
    Entity::Add<GameLogic>(picture2ID)->SetByInspector(material);
    Entity::Add<RectangleCollider>(picture2ID)->SetSize({ 1, 1 });
    Entity::Get<RectangleCollider>(picture2ID)->onEnter.Add(HelloEnter);
    Entity::Get<RectangleCollider>(picture2ID)->onExit.Add(HelloExit);

    Log(Entity::Get<RectangleCollider>(picture2ID)->to_string());

    auto circle1ID = Entity::Make("circle 1");
    Entity::Add<Transform>(circle1ID);
    Entity::Add<CircleCollider>(circle1ID)->SetLocalRadius(0.5f);

    auto circle2ID = Entity::Make("circle 2");
    Entity::Add<Transform>(circle2ID);
    Entity::Add<CircleCollider>(circle2ID)->SetLocalRadius(0.5f);
    Entity::Get<CircleCollider>(circle2ID)->onEnter.Add(HelloEnter);
    Entity::Get<CircleCollider>(circle2ID)->onExit.Add(HelloExit);

    Log(Entity::Get<CircleCollider>(circle2ID)->to_string());

    // ---------- saved scene data ---------- 

    Entity::Get<Transform>(picture1ID)->localPosition = { -0.5f, -0.5f, 0 };
    Entity::Get<Transform>(picture2ID)->localPosition = { 1.5f, -0.5f, 0 };
    Entity::Get<Transform>(circle1ID)->localPosition = { -0.5f,  0.6f, 0 };
    Entity::Get<Transform>(circle2ID)->localPosition = { 0.7f,  0.5f, 0 };

    entities.push_back(picture1ID);
    entities.push_back(picture2ID);
    entities.push_back(circle1ID);
    entities.push_back(circle2ID);

    /*
    Entity camera("camera");
    camera.AddComponent<Transform>();
    camera.AddComponent<Camera>()->SetOrthographicProjection();
    entities.push_back(camera);

    Entity picture1("picture 1");
    picture1.AddComponent<Transform>();
    picture1.AddComponent<Renderable>()->SetByInspector(mesh, material);
    picture1.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });
    entities.push_back(picture1);

    Entity picture2("picture 2");
    picture2.AddComponent<Transform>()->SetParent(picture1.GetComponent<Transform>());
    picture2.AddComponent<Renderable>()->SetByInspector(mesh, material);
    picture2.AddComponent<GameLogic>()->SetByInspector(material);
    picture2.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });
    picture2.GetComponent<RectangleCollider>()->onEnter.Add([&](Collider* other) { HelloEnter(other); });
    picture2.GetComponent<RectangleCollider>()->onExit.Add([&](Collider* other) { HelloExit(other); });
    entities.push_back(picture2);

    Entity circle1("circle 1");
    circle1.AddComponent<Transform>();
    circle1.AddComponent<CircleCollider>()->SetLocalRadius(0.5f);
    entities.push_back(circle1);

    Entity circle2("circle 2");
    circle2.AddComponent<Transform>();
    circle2.AddComponent<CircleCollider>()->SetLocalRadius(0.5f);
    circle2.GetComponent<CircleCollider>()->onEnter.Add([&](Collider* other) { HelloEnter(other); });
    circle2.GetComponent<CircleCollider>()->onExit.Add([&](Collider* other) { HelloExit(other); });
    entities.push_back(circle2);

    // ---------- saved scene data ---------- 
    picture1.GetComponent<Transform>()->localPosition = { -0.5f, -0.5f, 0 };
    picture2.GetComponent<Transform>()->localPosition = { 1.5f, -0.5f, 0 };
    circle1.GetComponent<Transform>()->localPosition = { -0.5f,  0.6f, 0 };
    circle2.GetComponent<Transform>()->localPosition = { 0.7f,  0.5f, 0 };
    */


    return entities;
}





