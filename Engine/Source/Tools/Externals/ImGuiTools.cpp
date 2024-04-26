#include "ImGuiTools.h"
#include "ShortHands.h"
Shorts;

namespace ImGui
{

	vec2 ToGlm(ImVec2 vec) { return vec2(vec.x, vec.y); }
	ImVec2 FromGlm(vec2 vec) { return ImVec2(vec.x, vec.y); }
    vec4 ToGlm(ImU32 color)
    {
        return { 
            (color & 0xFF) / 255.0f,
            ((color >> 8) & 0xFF) / 255.0f,
            ((color >> 16) & 0xFF) / 255.0f,
            ((color >> 24) & 0xFF) / 255.0f,
            };
    }
    ImU32 FromGlm(vec4 color)
    {
        return IM_COL32(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
    }


	bool Combo(const char* label, int* currentIndex, vector<string>& items, ImGuiComboFlags flags)
	{
        int lastIndex = *currentIndex;
        const char* combo_preview_value = items.empty() ? "No options" : items[*currentIndex].c_str();

        if (ImGui::BeginCombo(label, combo_preview_value, flags))
        {
            for (int n = 0; n < (int)items.size(); n++)
            {
                const bool is_selected = (*currentIndex == n);
                if (ImGui::Selectable(items[n].c_str(), is_selected))
                    *currentIndex = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        return *currentIndex != lastIndex;
	}



};


namespace logger
{
	string to_string(ImVec2 vec)
	{
		return "ImVec2(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
	}
};
