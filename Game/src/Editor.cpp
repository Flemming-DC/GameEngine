#include "Editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


namespace Editor
{


    void unused_TransformGUI(const Entity& entity, glm::vec3* eulerAngles)
    {
        auto name = entity.name;
        Transform& transform = Entity::Get<Transform>(entity.GetID()); // entity.GetComponent<Transform>();
        ImGui::Text(name.c_str());
        auto pointerToTranslation = &transform.localPosition.x;
        auto pointerToangles = &eulerAngles->x;
        Log("unused_TransformGUI WARNING: the gui modifies the transform.localRotation");
        auto pointerToScale = &transform.localScale.x;
        ImGui::SliderFloat3((name + ".translation").c_str(), pointerToTranslation, -2, 2); // imGUI requires unique widget-names
        ImGui::SliderFloat3((name + ".Rotaion").c_str(), pointerToangles, 0, 360);
        ImGui::SliderFloat3((name + ".Scale").c_str(), pointerToScale, 0.001f, 3);

        transform.localRotation = glm::quat(glm::radians(*eulerAngles));
    }

    void TransformGUI2D(const Entity& entity)
    {
        auto name = entity.name;
        Transform& transform = Entity::Get<Transform>(entity.GetID()); // entity.GetComponent<Transform>();
        ImGui::Text(name.c_str());
        auto pointerToTranslation = &transform.localPosition.x;
        float angle = glm::degrees(glm::eulerAngles(transform.localRotation)).z;
        //auto pointerToangles = &eulerAngles->z; // we point to the z component
        auto pointerToScale = &transform.localScale.x;
        ImGui::SliderFloat2((name + ".translation").c_str(), pointerToTranslation, -2, 2); // imGUI requires unique widget-names
        ImGui::SliderFloat((name + ".Rotaion").c_str(), &angle, 0, 360);
        ImGui::SliderFloat2((name + ".Scale").c_str(), pointerToScale, 0.001f, 3);

        //transform.localRotation = glm::quat(glm::radians(*eulerAngles));
        transform.localRotation = glm::quat(glm::radians(glm::vec3(0.0f, angle, 0.0f)));
    }

    void Update()
    {
        Editor::TransformGUI2D(Entity::register_.Get(Entity::GetID("camera")));
        Editor::TransformGUI2D(Entity::register_.Get(Entity::GetID("picture 1")));
        Editor::TransformGUI2D(Entity::register_.Get(Entity::GetID("picture 2")));
        Editor::TransformGUI2D(Entity::register_.Get(Entity::GetID("circle 1")));
        Editor::TransformGUI2D(Entity::register_.Get(Entity::GetID("circle 2")));
    }
};



