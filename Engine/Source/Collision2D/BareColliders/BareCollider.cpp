#include "BareCollider.h"

Shorts;

ITransform BareCollider::MakeTransform(vec2 pos, quat rot, vec2 scale)
{
	mat4 rotMatrix = glm::mat4_cast(rot);
	mat4 model = glm::translate(glm::mat4(1.0f), glm::ToVec3(pos))
		* rotMatrix
		* glm::scale(glm::mat4(1.0f), glm::ToVec3(scale, 1.0f));
	auto PosToWorldSpace = [model, rotMatrix](vec2 vec)
	{
		vec4 out4 = model * vec4(vec.x, vec.y, 0, 1);
		vec2 out2 = (vec2)out4;
		return out2;
	};
	auto NonPosToWorldSpace = [model, rotMatrix](vec2 vec)
	{
		vec4 out4 = rotMatrix * vec4(vec.x, vec.y, 0, 1);
		vec2 out2 = (vec2)out4;
		return out2;
	};
	Deny(glm::HasNAN(model), "model has nan: ", model);

	return {
		[pos]() { return glm::ToVec3(pos); },
		[scale]() { return glm::ToVec3(scale); },
		PosToWorldSpace,
		NonPosToWorldSpace
		};
}








