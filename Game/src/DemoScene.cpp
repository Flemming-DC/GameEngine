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

void DemoScene::HelloEnter(Collider* other)
{
    Log("DemoScene::Enter " + other->GetEntity()->name);
}
void DemoScene::HelloExit(Collider* other)
{
    Log("DemoScene::Exit " + other->GetEntity()->name);
}


std::vector<Entity> DemoScene::MakeEntities()
{
    // ---------- assets ---------- 
    Mesh mesh = Mesh::CreateSquare();

    glm::vec4 color = { 0.8f, 0.3f, 0.8f, 1.0f };
    Shader shader("res/shaders/Image.shader");
    Texture texture("res/textures/blizzard attacking fans.png");
    std::map<std::string, std::any> uniformsByName = {
        {"u_textureSampler", &texture},
        {"u_color", color},
        {"u_MVP", Material::MissingUniform()}
    };
    Material material(shader, uniformsByName);



    // ---------- proper scene stuff ---------- 
    std::vector<Entity> entitites;

    Entity camera("camera");
    camera.AddComponent<Transform>();
    camera.AddComponent<Camera>()->SetOrthographicProjection();
    entitites.push_back(camera);

    Entity picture1("picture 1");
    picture1.AddComponent<Transform>();
    picture1.AddComponent<Renderable>()->SetByInspector(&mesh, &material);
    picture1.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });
    entitites.push_back(picture1);

    Entity picture2("picture 2");
    picture2.AddComponent<Transform>()->SetParent(picture1.GetComponent<Transform>());
    picture2.AddComponent<Renderable>()->SetByInspector(&mesh, &material);
    picture2.AddComponent<GameLogic>()->SetByInspector(&material); // this is affecting both pictures due to shared material.
    picture2.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });
    picture2.GetComponent<RectangleCollider>()->onEnter.Add([&](Collider* other) { HelloEnter(other); });
    picture2.GetComponent<RectangleCollider>()->onExit.Add([&](Collider* other) { HelloExit(other); });
    entitites.push_back(picture2);

    Entity circle1("circle 1");
    circle1.AddComponent<Transform>();
    circle1.AddComponent<CircleCollider>()->SetLocalRadius(0.5f);
    entitites.push_back(circle1);

    Entity circle2("circle 2");
    circle2.AddComponent<Transform>();
    circle2.AddComponent<CircleCollider>()->SetLocalRadius(0.5f);
    circle2.GetComponent<CircleCollider>()->onEnter.Add([&](Collider* other) { HelloEnter(other); });
    circle2.GetComponent<CircleCollider>()->onExit.Add([&](Collider* other) { HelloExit(other); });
    entitites.push_back(circle2);

    // ---------- saved scene data ---------- 
    picture1.GetComponent<Transform>()->localPosition = { -0.5f, -0.5f, 0 };
    picture2.GetComponent<Transform>()->localPosition = { 1.5f, -0.5f, 0 };
    circle1.GetComponent<Transform>()->localPosition = { -0.5f,  0.6f, 0 };
    circle2.GetComponent<Transform>()->localPosition = { 0.7f,  0.5f, 0 };
    // polygon collider data also goes here, if you need them

    return entitites;
}





