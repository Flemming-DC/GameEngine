#include "ImGuiTools.h"

namespace ImGui
{
	glm::vec2 ToGlm(ImVec2 vec) { return glm::vec2(vec.x, vec.y); }
	ImVec2 FromGlm(glm::vec2 vec) { return ImVec2(vec.x, vec.y); }


	bool Combo(const char* label, int* currentIndex, std::vector<std::string>& items, ImGuiComboFlags flags)
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
	std::string to_string(ImVec2 vec)
	{
		return "ImVec2(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
	}
};
