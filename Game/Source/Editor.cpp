#include "Editor.h"
#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw_gl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "GameAssets.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Input.h"

// make viewport framebuffer size sensitive to my viewport vindow size.



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
void SceneEditor()
{
    auto renderResult = Renderer::DrawToFrameBuffer();
        
    ImVec2 uvBottumLeft = { 0, 1 };
    ImVec2 uvTopRight = { 1, 0 };
    ImVec2 size = { (float)renderResult.width, (float)renderResult.height };

    ImGui::Begin("ViewPort");
    ImGui::Image((void*)(intptr_t)renderResult.textureOpenGlid, size, uvBottumLeft, uvTopRight);
    ImGui::End();
}
void GameView() {}
void AssetFolder() {}


void Hierarchy()
{
    std::string ident = "   ";
    ImGui::Begin("Hierarchy");

    ImGui::Text((ident + "camera").c_str());
    if (ImGui::TreeNode("picture 1")) // tree nodes are for parents
    {
        ImGui::Text((ident + "picture 2").c_str()); // text are for leaves
        ImGui::TreePop();
    }
    ImGui::Text((ident + "circle 1").c_str());
    ImGui::Text((ident + "circle 2").c_str());

        
    static int selected = -1;
    for (int n = 0; n < 5; n++)
    {
        //char buf[32];
        //sprintf(buf, "Object %d", n);
        std::string str = "Object " + std::to_string(n);
        if (ImGui::Selectable(str.c_str(), selected == n))
            selected = n;
    }

    ImGui::Text(" ------ Selectable nodes ------ ");

    static ImGuiTreeNodeFlags base_flags =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth;

    // 'selection_mask' is dumb representation of what may be user-side selection state.
    //  You may retain selection state inside or outside your objects in whatever format you see fit.
    // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
    /// of the loop. May be a pointer to your own node type, etc.
    static int selection_mask = (1 << 2);
    int node_clicked = -1;
    for (int i = 0; i < 6; i++)
    {
        // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
        // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow 
        // doesn't alter selection.
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        if (i < 3)
        {
            // Items 0..2 are Tree Node
            bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = i;
            if (node_open)
            {
                ImGui::BulletText("Blah blah\nBlah Blah");
                ImGui::TreePop();
            }
        }
        else
        {
            // Items 3..5 are Tree Leaves
            // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
            // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
            ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = i;
        }
    }
    if (node_clicked != -1)
    {
        // Update selection state
        // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
            selection_mask = (1 << node_clicked);           // Click to single-select
    }


    /*
    if (ImGui::TreeNode("Basic trees"))
    {
        for (int i = 0; i < 5; i++)
        {
            // Use SetNextItemOpen() so set the default state of a node to be open. We could
            // also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
            if (i == 0)
                ImGui::SetNextItemOpen(true, ImGuiCond_Once); // ImGuiCond_Once = 2

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
    */
    ImGui::End();
}



void Editor::OnUpdate()
{
    for (const auto& entity : Entity::register_.GetData())
        TransformGUI2D(entity);

    Inspector();
    SceneEditor();
    GameView();
    AssetFolder();
    //Hierarchy();

    //ImGui::ShowDemoWindow();

    if (Input::KeyHeldDown(Keyboard::exc))
        Initializer::Exit(); // save first
}


