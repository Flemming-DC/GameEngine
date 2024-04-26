#include "GlmCheck.h"
#include "ErrorChecker.h"


namespace GlmCheck
{
	const float unRealisticallySmall = glm::pow(10.0f, -10.0f);
	const float unRealisticallyLarge = glm::pow(10.0f, 10.0f);

	float _Check(float toCheck, bool zeroable, bool signed_)
	{
		if (glm::isnan(toCheck))
			RaiseError("nan encountered");
		if (glm::abs(toCheck) > unRealisticallyLarge)
			RaiseError(toCheck, " is unrealistically large");
		if (!zeroable && glm::abs(toCheck) < unRealisticallySmall)
			RaiseError(toCheck, " is unrealistically small");
		if (!signed_ && toCheck < 0)
			RaiseError(toCheck, " should be non-negative");
		//if (glm::isinf(num))
		//	RaiseError("inf encountered");
		return toCheck;
	}



};
