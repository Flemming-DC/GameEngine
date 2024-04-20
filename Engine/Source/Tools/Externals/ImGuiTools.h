#pragma once
#include "GlmTools.h"
#include "imgui/imgui.h"
#include <vector>
#include <string>

namespace ImGui
{
	glm::vec2 ToGlm(ImVec2 vec);
	ImVec2 FromGlm(glm::vec2 vec);
	bool Combo(const char* label, int* currentIndex, std::vector<std::string>& items, ImGuiComboFlags flags = 0);

};


namespace logger
{
	std::string to_string(ImVec2 vec);
};

