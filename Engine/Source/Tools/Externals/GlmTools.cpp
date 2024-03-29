#include "GlmTools.h"

namespace Tools
{

	bool HasNAN(glm::vec2 vec)
	{
		return glm::isnan(vec.x) || glm::isnan(vec.y);
	}
	bool HasNAN(glm::vec3 vec)
	{
		return glm::isnan(vec.x) || glm::isnan(vec.y) || glm::isnan(vec.z);

	}
	bool HasNAN(glm::vec4 vec)
	{
		return glm::isnan(vec.r) || glm::isnan(vec.g) || glm::isnan(vec.b) || glm::isnan(vec.a);

	}
	bool HasNAN(glm::mat4 mat)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (glm::isnan(mat[j][i]))
					return true;
			}
		}
		return false;
	}
}


namespace logger
{

	std::string to_string(glm::vec2 vec)
	{
		return "vec2(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
	}

	std::string to_string(glm::vec3 vec)
	{
		return "vec3(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
	}

	std::string to_string(glm::vec4 vec)
	{
		return "vec4("
			+ std::to_string(vec.r) + ", "
			+ std::to_string(vec.g) + ", "
			+ std::to_string(vec.b) + ", "
			+ std::to_string(vec.a) + ", "
			+ ")";
	}

	std::string to_string(glm::mat4 mat)
	{
		std::string out = "mat4(";
		for (int i = 0; i < 4; i++)
		{
			out += "\n    ";
			for (int j = 0; j < 4; j++)
				out += std::to_string(mat[j][i]) + ", ";
		}
		out += ")";
		return out;
	}
}


