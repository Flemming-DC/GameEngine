#include "GlmCheck.h"
#include "ErrorChecker.h"


namespace GlmCheck
{
	// realistic numbers are assumed to lie in [realisticallySmall, realisticallyLarge]
	// numbers outside [unRealisticallySmall, unRealisticallyLarge] are deemed unrealistic and assumed to be caused by bugs
	const float unRealisticallySmall = glm::pow(10.0f, -10.0f);
	const float unRealisticallyLarge = glm::pow(10.0f, 10.0f);
	const float realisticallySmall = glm::pow(10.0f, -6.0f);
	const float realisticallyLarge = glm::pow(10.0f, 6.0f);

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
		if (glm::isnan(toCheck))
			RaiseError(variableName, " is nan.");
		if (glm::abs(toCheck) > unRealisticallyLarge)
			RaiseError(variableName, " = ", toCheck, ", which is unrealistically large.");
		if (!zeroable && glm::abs(toCheck) < unRealisticallySmall)
			RaiseError(variableName, " = ", toCheck, ", which is unrealistically small.");
		if (!signed_ && toCheck < 0)
			RaiseError(variableName, " = ", toCheck, ", but is not allowed to be negative.");
		//if (glm::isinf(num))
		//	RaiseError("inf encountered");
		return toCheck;
	}



};
