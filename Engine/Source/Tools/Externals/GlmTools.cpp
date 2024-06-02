#include "GlmTools.h"
#include "ShortHands.h"
#include "Time_.h"


namespace glm
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


	glm::vec3 ToVec3(glm::vec2 vec, float z)
	{
		return glm::vec3(vec.x, vec.y, z);
	}

	glm::quat NoRotation()
	{
		return glm::identity<glm::quat>();
	}

	float SmoothAngle(float current, float target, float duration)
	{
		// correct for discontinuity in angles
		if (target - current > glm::PI)
			target -= 2 * glm::PI;
		else if (target - current < -glm::PI)
			target += 2 * glm::PI;

		return SmoothFloat(current, target, duration);
	}

	vec2 SmoothVec(vec2 current, vec2 target, float duration)
	{
		return glm::vec2(
			SmoothFloat(current.x, target.x, duration),
			SmoothFloat(current.y, target.y, duration));
	}

	float SmoothFloat(float current, float target, float duration)
	{
		return target - (target - current) * glm::exp(-Time::Delta() * 10 / duration);
	}

	vec2 PolarVec2(float radius, float angle)
	{
		return vec2(
			radius * glm::cos(angle),
			radius * glm::sin(angle)
			);
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


	std::string to_string(glm::quat rot)
	{
		/*
		return "quat("
			+ std::to_string(rot.w) + ", "
			+ std::to_string(rot.x) + ", "
			+ std::to_string(rot.y) + ", "
			+ std::to_string(rot.z) + ", "
			+ ")";*/
		glm::vec3 euler = glm::eulerAngles(rot);
		return "quat.euler("
			+ std::to_string(euler.x) + ", "
			+ std::to_string(euler.y) + ", "
			+ std::to_string(euler.z) + ", "
			+ ")";
	}
}


