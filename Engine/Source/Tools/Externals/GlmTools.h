#pragma once
#include <string>
#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>

namespace glm
{
	
	bool HasNAN(glm::vec2 vec);
	bool HasNAN(glm::vec3 vec);
	bool HasNAN(glm::vec4 vec);
	bool HasNAN(glm::mat4 mat);

	glm::vec3 ToVec3(glm::vec2 vec, float z = 0);
	glm::quat NoRotation();

	inline bool LargerThan(glm::vec2 v1, glm::vec2 v2) { return glm::dot(v1, v1) > glm::dot(v2, v2); }
	inline bool LargerThan(glm::vec3 v1, glm::vec3 v2) { return glm::dot(v1, v1) > glm::dot(v2, v2); }
	inline bool LargerThan(glm::vec4 v1, glm::vec4 v2) { return glm::dot(v1, v1) > glm::dot(v2, v2); }
	inline bool LessThan(glm::vec2 v1, glm::vec2 v2) { return glm::dot(v1, v1) < glm::dot(v2, v2); }
	inline bool LessThan(glm::vec3 v1, glm::vec3 v2) { return glm::dot(v1, v1) < glm::dot(v2, v2); }
	inline bool LessThan(glm::vec4 v1, glm::vec4 v2) { return glm::dot(v1, v1) < glm::dot(v2, v2); }

	inline bool LargerThan(glm::vec2 vec, float threshold) { return glm::dot(vec, vec) > threshold * threshold; }
	inline bool LargerThan(glm::vec3 vec, float threshold) { return glm::dot(vec, vec) > threshold * threshold; }
	inline bool LargerThan(glm::vec4 vec, float threshold) { return glm::dot(vec, vec) > threshold * threshold; }
	inline bool LessThan(glm::vec2 vec, float threshold) { return glm::dot(vec, vec) < threshold * threshold; }
	inline bool LessThan(glm::vec3 vec, float threshold) { return glm::dot(vec, vec) < threshold * threshold; }
	inline bool LessThan(glm::vec4 vec, float threshold) { return glm::dot(vec, vec) < threshold * threshold; }
	
	inline float SqrMagnitude(glm::vec2 vec) { return glm::dot(vec, vec); }
	inline float SqrMagnitude(glm::vec3 vec) { return glm::dot(vec, vec); }
	inline float SqrMagnitude(glm::vec4 vec) { return glm::dot(vec, vec); }

	inline float Magnitude(glm::vec2 vec) { return glm::length(vec); }
	inline float Magnitude(glm::vec3 vec) { return glm::length(vec); }
	inline float Magnitude(glm::vec4 vec) { return glm::length(vec); }

}

namespace logger
{

	std::string to_string(glm::vec2 vec);
	std::string to_string(glm::vec3 vec);
	std::string to_string(glm::vec4 vec);
	std::string to_string(glm::mat4 mat);
	std::string to_string(glm::quat rot);
}




