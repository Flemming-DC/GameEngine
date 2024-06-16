#include "BareCollider.h"

Shorts;

bool BoundingBox::IsOverlapping(const BoundingBox& other)
{
	if (this->maxX < other.minX) return false;
	if (this->minX > other.maxX) return false;
	if (this->maxY < other.minY) return false;
	if (this->minY > other.maxY) return false;

	return true;
}
bool BoundingBox::IsOverlappingX(const BoundingBox& other)
{
	if (this->maxX < other.minX) return false;
	if (this->minX > other.maxX) return false;

	return true;
}
bool BoundingBox::IsOverlappingY(const BoundingBox& other)
{
	if (this->maxY < other.minY) return false;
	if (this->minY > other.maxY) return false;

	return true;
}

ITransform BareCollider::MakeTransform(vec2 pos, quat rot, vec2 scale)
{
	mat4 rot_matrix = glm::mat4_cast(rot);
	mat4 model = glm::translate(glm::mat4(1.0f), glm::ToVec3(pos))
		* rot_matrix
		* glm::scale(glm::mat4(1.0f), glm::ToVec3(scale, 1.0f));
	auto ToWorldSpace = [model, rot_matrix](vec2 vec, bool isPos)
	{
		mat4 matrix = isPos ? model : rot_matrix;
		vec4 out4 = matrix * vec4(vec.x, vec.y, 0, 1);
		vec2 out2 = (vec2)out4;
		return out2;
	};
	Deny(glm::HasNAN(model),
		"model has nan: ", model);

	return {
		[pos]() { return glm::ToVec3(pos); },
		[scale]() { return glm::ToVec3(scale); },
		ToWorldSpace
		};
}








