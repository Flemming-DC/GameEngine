#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "Initializer.h"
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
#include "Register.h"
#include "UuidCreator.h"
#include "GameAssets.h"
#include "EngineAssets.h"
#include "DemoScene.h"
#include "ListTools.h"


void unused_TransformGUI(const Entity& entity, glm::vec3* eulerAngles)
{
    auto name = entity.name;
    Transform& transform = Entity::Get<Transform>(entity.GetID()); // entity.GetComponent<Transform>();
    ImGui::Text(name.c_str());
    auto pointerToTranslation = &transform.localPosition.x;
    auto pointerToangles = &eulerAngles->x;
    auto pointerToScale = &transform.localScale.x;
    ImGui::SliderFloat3((name + ".translation").c_str(), pointerToTranslation, -2, 2); // imGUI requires unique widget-names
    ImGui::SliderFloat3((name + ".Rotaion").c_str(), pointerToangles, 0, 360);
    ImGui::SliderFloat3((name + ".Scale").c_str(), pointerToScale, 0.001f, 3);

    transform.localRotation = glm::quat(glm::radians(*eulerAngles));
}

void TransformGUI2D(const Entity& entity, glm::vec3* eulerAngles)
{
    auto name = entity.name;
    Transform& transform = Entity::Get<Transform>(entity.GetID()); // entity.GetComponent<Transform>();
    ImGui::Text(name.c_str());
    auto pointerToTranslation = &transform.localPosition.x;
    auto pointerToangles = &eulerAngles->z; // we point to the z component
    auto pointerToScale = &transform.localScale.x;
    ImGui::SliderFloat2((name + ".translation").c_str(), pointerToTranslation, -2, 2); // imGUI requires unique widget-names
    ImGui::SliderFloat((name + ".Rotaion").c_str(), pointerToangles, 0, 360);
    ImGui::SliderFloat2((name + ".Scale").c_str(), pointerToScale, 0.001f, 3);

    transform.localRotation = glm::quat(glm::radians(*eulerAngles));
}

void HelloEnter(Collider* other)
{
    Log("Enter " + other->GetEntity().name);
}
void HelloExit(Collider* other)
{
    Log("Exit " + other->GetEntity().name);
}




void run()
{

    Initializer::Setup();
    Renderer::SetupGrid2D(0.25f);
    GameAssets::Setup();

    // ---------- assets ---------- 
    Mesh& mesh = EngineAssets::SquareMesh();
    Material& material = GameAssets::GetMaterial();
    

    // ---------- cpp scene data ---------- 
    /*
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


    auto circle1ID = Entity::Make("circle 1");
    Entity::Add<Transform>(circle1ID);
    Entity::Add<CircleCollider>(circle1ID)->SetLocalRadius(0.5f);

        auto circle2ID = Entity::Make("circle 2");
    Entity::Add<Transform>(circle2ID);
    Entity::Add<CircleCollider>(circle2ID)->SetLocalRadius(0.5f);
    Entity::Get<CircleCollider>(circle2ID)->onEnter.Add(HelloEnter);
    Entity::Get<CircleCollider>(circle2ID)->onExit.Add(HelloExit);

    // ---------- saved scene data ---------- 
    
    Entity::Get<Transform>(picture1ID)->localPosition = { -0.5f, -0.5f, 0 };
    Entity::Get<Transform>(picture2ID)->localPosition = { 1.5f, -0.5f, 0 };
    Entity::Get<Transform>(circle1ID)->localPosition = { -0.5f,  0.6f, 0 };
    Entity::Get<Transform>(circle2ID)->localPosition = { 0.7f,  0.5f, 0 };

    */

    /*
    for (const auto& pair : Entity::componentsByID)
    {
        auto id = UuidCreator::to_string(pair.first);
        auto c = Tools::to_string(pair.second);
        Log(id + ": " + c);
    }

    */

    DemoScene demoScene;// = DemoScene("demo");
    //std::vector<uuids::uuid>& entities = demoScene.MakeEntities();
    demoScene.MakeEntities();

    //LoadSpatialData(entities);

    // ------------ loop ------------

    glm::vec3 eulerAngles1(0);
    glm::vec3 eulerAngles2(0);
    glm::vec3 eulerAnglesCircle1(0);
    glm::vec3 eulerAnglesCircle2(0);

    Log("--------- starting loop --------- ");
    while (Initializer::NewFrame())
    {
        CollisionLoop::Update();

        // --------- custom logic start -----------
        TransformGUI2D(demoScene.GetEntity(0), &eulerAngles1);
        TransformGUI2D(demoScene.GetEntity(1), &eulerAngles2);
        TransformGUI2D(demoScene.GetEntity(2), &eulerAnglesCircle1);
        TransformGUI2D(demoScene.GetEntity(3), &eulerAnglesCircle2);
        /*
        TransformGUI2D(picture1, &eulerAngles1);
        TransformGUI2D(picture2, &eulerAngles2);
        TransformGUI2D(circle1, &eulerAnglesCircle1);
        TransformGUI2D(circle2, &eulerAnglesCircle2);
        */
        Entity::UpdateAllEntities();

        if (Input::KeyHeldDown(GLFW_KEY_K))
        {
            Log("K");
            for (const auto& overlap : CollisionChecker::GetOverlaps())
                Log(overlap.first->GetEntity().name + " overlaps " + overlap.second->GetEntity().name);
        }
        if (Input::KeyHeldDown(GLFW_KEY_L))
        {
            Log("L");
            auto colliders = CollisionChecker::RayOverlaps(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
            for (const auto& col : colliders)
                Log(col->GetEntity().name + " was hit");
        }
        if (Input::KeyHeldDown(GLFW_KEY_P))
        {
            Log("P");
            auto collider = CollisionChecker::RayCast(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
            if (collider != nullptr)
                Log(collider->GetEntity().name + " was hit");
            else
                Log("Hit nothing");
        }
        // --------- custom logic end -----------

        Renderer::Draw();
        Initializer::EndFrame();
    }
    Log("------- Shutdown --------");
    Initializer::Shutdown();
}


int main()
{
    run();
    return 0;
}
// git add . && git commit -m "m" && git push
