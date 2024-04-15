#include "ImGuiTools.h"

namespace ImGui
{
	glm::vec2 ToGlm(ImVec2 vec) { return glm::vec2(vec.x, vec.y); }
	ImVec2 FromGlm(glm::vec2 vec) { return ImVec2(vec.x, vec.y); }
};


namespace logger
{
	std::string to_string(ImVec2 vec)
	{
		return "ImVec2(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
	}
};
