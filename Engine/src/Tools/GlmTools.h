#pragma once
#include <string>
#include "glm/glm.hpp"

namespace Tools
{

	bool HasNAN(glm::vec2 vec);
	bool HasNAN(glm::vec3 vec);
	bool HasNAN(glm::vec4 vec);
	bool HasNAN(glm::mat4 mat);


}

namespace logger
{

	std::string to_string(glm::vec2 vec);
	std::string to_string(glm::vec3 vec);
	std::string to_string(glm::vec4 vec);
	std::string to_string(glm::mat4 mat);
}




