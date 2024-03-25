#pragma once
#include <utility>
#include "GlmTools.h"

class Screen
{
public:
	static glm::vec3 ToWorldPosition(std::pair<double, double> screenPosition);
	static std::pair<double, double> FromWorldPosition(glm::vec3 worldPosition);
};

