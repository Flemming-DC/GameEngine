#pragma once
#include "GlmTools.h"
#include "imgui/imgui.h"

namespace ImGui
{
	glm::vec2 ToGlm(ImVec2 vec);
	ImVec2 FromGlm(glm::vec2 vec);
};


namespace logger
{
	std::string to_string(ImVec2 vec);
};

