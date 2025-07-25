#include "GlmCheck.h"
#include "ErrorChecker.h"


namespace GlmCheck
{
	// realistic numbers are assumed to lie in [realisticallySmall, realisticallyLarge]
	// numbers outside [unRealisticallySmall, unRealisticallyLarge] are deemed unrealistic and assumed to be caused by bugs
	const float unRealisticallySmall = glm::pow(10.0f, -10.0f);
	const float unRealisticallyLarge = glm::pow(10.0f, 10.0f);

	float _Realistic(float toMakeSafe, bool signed_)
	{
		// toMakeSafe >= 0, !signed_ are not equivalent, since data can be incorrect
		// the whole point of this function is to correct imperfect data.
		if (toMakeSafe >= 0 || !signed_)
			return glm::clamp(toMakeSafe, realisticallySmall, realisticallyLarge);
		else
			return glm::clamp(toMakeSafe, -realisticallyLarge, -realisticallySmall);
	}


	float _Check(std::string variableName, float toCheck, bool zeroable, bool signed_)
	{
		Deny(glm::isnan(toCheck),
			variableName, " is nan.");
		Deny(glm::abs(toCheck) > unRealisticallyLarge,
			variableName, " = ", toCheck, ", which is unrealistically large.");
		Deny(!zeroable && glm::abs(toCheck) < unRealisticallySmall,
			variableName, " = ", toCheck, ", which is unrealistically small.");
		Deny(!signed_ && toCheck < 0,
			variableName, " = ", toCheck, ", but is not allowed to be negative.");
		//if (glm::isinf(num))
		//	RaiseError("inf encountered");
		return toCheck;
	}


	glm::vec2 _CheckVec(std::string variableName, glm::vec2 toCheck)
	{
		_Check(variableName, glm::Magnitude(toCheck), false, false);
		return glm::vec2(
			_Check(variableName, toCheck.x, true, true),
			_Check(variableName, toCheck.y, true, true)
			);
	}

};
