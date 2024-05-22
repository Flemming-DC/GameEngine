#pragma once
#include "GlmTools.h"
#include "imgui/imgui.h"
#include <vector>
#include <string>

namespace ImGui
{
	// returns true if text were changed
	bool AutoCompletor(const char* label, std::string* strInput, std::vector<std::string>& completionOptions, bool focus = false);
	//std::vector<std::string> autoCompletionCandidatex
};
