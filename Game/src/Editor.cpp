#include "Editor.h"
#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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

    void Inspector() {}
    void SceneEditor() {}
    void GameView() {}
    void AssetFolder() {}
    void Hierarchy()
    {

        if (ImGui::TreeNode("Basic trees"))
        {
            for (int i = 0; i < 5; i++)
            {
                // Use SetNextItemOpen() so set the default state of a node to be open. We could
                // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
                //if (i == 0)
                    //ImGui::SetNextItemOpen(true); // ImGuiCond_Once = 2

                if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                {
                    ImGui::Text("blah blah");
                    ImGui::SameLine();
                    if (ImGui::SmallButton("button")) {}
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
    }

    void Update()
    {
        TransformGUI2D(Entity::register_.Get(Entity::GetID("camera")));
        TransformGUI2D(Entity::register_.Get(Entity::GetID("picture 1")));
        TransformGUI2D(Entity::register_.Get(Entity::GetID("picture 2")));
        TransformGUI2D(Entity::register_.Get(Entity::GetID("circle 1")));
        TransformGUI2D(Entity::register_.Get(Entity::GetID("circle 2")));

        void Inspector();
        void SceneEditor();
        void GameView();
        void AssetFolder();
        void Hierarchy();

        ImGui::ShowDemoWindow();
    }
};



