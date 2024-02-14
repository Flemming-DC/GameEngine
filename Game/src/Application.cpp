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
#include "ShaderRegister.h"
#include "UuidCreator.h"

void unused_TransformGUI(const Entity& entity, glm::vec3* eulerAngles)
{
    auto name = entity.name;
    Transform* transform = entity.GetComponent<Transform>();
    ImGui::Text(name.c_str());
    auto pointerToTranslation = &transform->localPosition.x;
    auto pointerToangles = &eulerAngles->x;
    auto pointerToScale = &transform->localScale.x;
    ImGui::SliderFloat3((name + ".translation").c_str(), pointerToTranslation, -2, 2); // imGUI requires unique widget-names
    ImGui::SliderFloat3((name + ".Rotaion").c_str(), pointerToangles, 0, 360);
    ImGui::SliderFloat3((name + ".Scale").c_str(), pointerToScale, 0.001f, 3);

    transform->localRotation = glm::quat(glm::radians(*eulerAngles));
}

void TransformGUI2D(const Entity& entity, glm::vec3* eulerAngles)
{
    auto name = entity.name;
    Transform* transform = entity.GetComponent<Transform>();
    ImGui::Text(name.c_str());
    auto pointerToTranslation = &transform->localPosition.x;
    auto pointerToangles = &eulerAngles->z; // we point to the z component
    auto pointerToScale = &transform->localScale.x;
    ImGui::SliderFloat2((name + ".translation").c_str(), pointerToTranslation, -2, 2); // imGUI requires unique widget-names
    ImGui::SliderFloat((name + ".Rotaion").c_str(), pointerToangles, 0, 360);
    ImGui::SliderFloat2((name + ".Scale").c_str(), pointerToScale, 0.001f, 3);

    transform->localRotation = glm::quat(glm::radians(*eulerAngles));
}

void HelloEnter(Collider* other)
{
    Log("Enter " + other->GetEntity()->name);
}
void HelloExit(Collider* other)
{
    Log("Exit " + other->GetEntity()->name);
}





void run()
{
    auto uuid = UuidCreator::GetUnInitializedID();
    Log(UuidCreator::to_string(uuid));

    Initializer::Setup();
    Renderer::SetupGrid2D(0.25f);
    
    // ---------- assets ---------- 
    Mesh mesh = Mesh::CreateSquare();

    glm::vec4 color = { 0.8f, 0.3f, 0.8f, 1.0f };

    auto id_ = Shader::register_.Add("res/shaders/Image.shader").GetID();
    const Shader& shader_ = Shader::register_.Get(id_);
    Shader::register_.Remove(id_);

    const Shader& shader = Shader::register_.Add("res/shaders/Image.shader");

    //Shader shader = Shader::register_.Add();
    //shader.Setup("res/shaders/Image.shader");

    
    Texture texture("res/textures/blizzard attacking fans.png");
    std::map<std::string, std::any> uniformsByName = {
        {"u_textureSampler", &texture},
        {"u_color", color},
        {"u_MVP", Material::MissingUniform()}
    };
    Material material(shader, uniformsByName);


    // ---------- cpp scene data ---------- 

    Entity camera("camera");
    camera.AddComponent<Transform>();
    camera.AddComponent<Camera>()->SetOrthographicProjection();


    Entity picture1("picture 1");
    picture1.AddComponent<Transform>();
    picture1.AddComponent<Renderable>()->SetByInspector(&mesh, &material); 
    picture1.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });

    Entity picture2("picture 2");
    picture2.AddComponent<Transform>()->SetParent(picture1.GetComponent<Transform>());
    picture2.AddComponent<Renderable>()->SetByInspector(&mesh, &material);
    picture2.AddComponent<GameLogic>()->SetByInspector(&material); // this is affecting both pictures due to shared material.
    picture2.AddComponent<RectangleCollider>()->SetSize({ 1, 1 });
    picture2.GetComponent<RectangleCollider>()->onEnter.Add(HelloEnter);
    picture2.GetComponent<RectangleCollider>()->onExit.Add(HelloExit);

    Entity circle1("circle 1");
    circle1.AddComponent<Transform>();
    circle1.AddComponent<CircleCollider>()->SetLocalRadius(0.5f);

    Entity circle2("circle 2");
    circle2.AddComponent<Transform>();
    circle2.AddComponent<CircleCollider>()->SetLocalRadius(0.5f);
    circle2.GetComponent<CircleCollider>()->onEnter.Add(HelloEnter);
    circle2.GetComponent<CircleCollider>()->onExit.Add(HelloExit);

    // ---------- saved scene data ---------- 
    picture1.GetComponent<Transform>()->localPosition = { -0.5f, -0.5f, 0 };
    picture2.GetComponent<Transform>()->localPosition = { 1.5f, -0.5f, 0 };
    circle1.GetComponent<Transform>()->localPosition = { -0.5f,  0.6f, 0 };
    circle2.GetComponent<Transform>()->localPosition = { 0.7f,  0.5f, 0 };
    

    // ------------ loop ------------

    glm::vec3 eulerAngles1(0);
    glm::vec3 eulerAngles2(0);
    glm::vec3 eulerAnglesCircle1(0);
    glm::vec3 eulerAnglesCircle2(0);

    Log("starting loop");
    while (Initializer::NewFrame())
    {
        CollisionLoop::Update();

        // --------- custom logic start -----------
        TransformGUI2D(picture1, &eulerAngles1);
        TransformGUI2D(picture2, &eulerAngles2);
        TransformGUI2D(circle1, &eulerAnglesCircle1);
        TransformGUI2D(circle2, &eulerAnglesCircle2);
        Entity::UpdateAllEntities();

        if (Input::KeyHeldDown(GLFW_KEY_K))
        {
            Log("K");
            for (const auto& overlap : CollisionChecker::GetOverlaps())
                Log(overlap.first->GetEntity()->name + " overlaps " + overlap.second->GetEntity()->name);
        }
        if (Input::KeyHeldDown(GLFW_KEY_L))
        {
            Log("L");
            auto colliders = CollisionChecker::RayOverlaps(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
            for (const auto& col : colliders)
                Log(col->GetEntity()->name + " was hit");
        }
        if (Input::KeyHeldDown(GLFW_KEY_P))
        {
            Log("P");
            auto collider = CollisionChecker::RayCast(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1);
            if (collider != nullptr)
                Log(collider->GetEntity()->name + " was hit");
            else
                Log("Hit nothing");
        }
        // --------- custom logic end -----------

        Renderer::Draw();
        Initializer::EndFrame();
    }
    Initializer::Shutdown();
}


int main()
{
    run();
    return 0;
}
// git add . && git commit -m "m" && git push
