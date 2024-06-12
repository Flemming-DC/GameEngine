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
	if (glm::HasNAN(model))
		RaiseError("model has nan: ", model);

	return {
		[pos]() { return glm::ToVec3(pos); },
		[scale]() { return glm::ToVec3(scale); },
		ToWorldSpace
		};
}



/*
glm::vec2 Transform::ToWorldSpace(glm::vec2 localVector2D, bool isPosition) const
{
	glm::mat4 matrix = isPosition ? GetLocalModel() : glm::mat4_cast(GetRotation());
	return matrix * glm::vec4(localVector2D.x, localVector2D.y, 0, 1);
}
glm::mat4 Transform::GetLocalModel() const
{
	return glm::translate(glm::mat4(1.0f), localPosition)
		* glm::mat4_cast(localRotation)
		* glm::scale(glm::mat4(1.0f), localScale);
}
struct ITransform // this struct serves as an interface for a transform.
{
	Shorts;
	function<vec3()> GetPosition;
	function<vec3()> GetScale;
	function<vec2(vec2, bool)> ToWorldSpace; // (localVector, isPositionvector) -> worldVector
};

BareCircleCollider MakeCircle(vec2 center, float radius)
{

}

BareCircleCollider MakePoint(vec2 position)
{

}

BarePolygonCollider MakePolygon(vector<vec2> positions)
{

}

BarePolygonCollider MakeRectangle(vec2 center, quat rot, vec2 size)
{

}
*/






