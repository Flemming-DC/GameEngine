#include "BarePolygonCollider.h"
#include "ErrorChecker.h"


BarePolygonCollider BarePolygonCollider::MakeRectangle(vec2 center, quat rot, vec2 size)
{
	vec2 halfSize = 0.5f * size;
	// this is inefficient, but probably not a bottleneck.
	vector<vec2> positions =
	{
		vec2(glm::ToVec3(center) + rot * vec3(-halfSize.x, -halfSize.y, 0.0f )), // LD
		vec2(glm::ToVec3(center) + rot * vec3(-halfSize.x,  halfSize.y, 0.0f )), // LU
		vec2(glm::ToVec3(center) + rot * vec3( halfSize.x,  halfSize.y, 0.0f )), // RU
		vec2(glm::ToVec3(center) + rot * vec3( halfSize.x, -halfSize.y, 0.0f )), // RD
	};
	return Make(positions);
}

BarePolygonCollider BarePolygonCollider::Make(vector<vec2> positions)
{
	BarePolygonCollider col;
	col.Setup(col.MakeTransform(), positions); // vec2(0.0f), quat(), vec2(1.0f)
	return col;
}

void BarePolygonCollider::Setup(ITransform iTransform_, std::vector<glm::vec2> localPosition2Ds_)
{
	iTransform = iTransform_,
	localPosition2Ds = localPosition2Ds_;

	// calculate additional data
	localNormals.clear();
	int count = (int)localPosition2Ds.size();
	for (int i = 0; i < count; i++)
	{
		int nextIndex = i + 1 < count ? i + 1 : 0;
		auto vectorAlongEdge = localPosition2Ds[nextIndex] - localPosition2Ds[i];
		auto perpendicular = glm::vec2(-vectorAlongEdge.y, vectorAlongEdge.x);
		localNormals.push_back(glm::normalize(perpendicular));
	}

	centerOfMass = glm::vec2(0);
	for (auto position2D : localPosition2Ds)
		centerOfMass += position2D;
	centerOfMass /= count;

	// if normals on average point inwards, then flip their sign
	float divergence = 0;
	for (int i = 0; i < count; i++)
	{
		glm::vec2 radialvector = glm::normalize(localPosition2Ds[i] - centerOfMass);
		divergence += glm::dot(localNormals[i], radialvector);
	}
	if (divergence < 0)
	{
		for (int i = 0; i < count; i++)
			localNormals[i] *= -1;
	}
}




std::pair<float, float> BarePolygonCollider::ShadowAlongNormal(glm::vec2 normal) const
{
	float min = INFINITY;
	float max = -INFINITY;
	for (auto localPosition2D : localPosition2Ds)
	{
		//glm::vec2 position2D = isLocal ? localPosition2D : GetTransform().ToWorldSpace(localPosition2D, true);
		glm::vec2 position2D = iTransform.ToWorldSpace(localPosition2D, true);
		float coordinateAlongNormal = glm::dot(position2D, normal);
		if (coordinateAlongNormal < min)
			min = coordinateAlongNormal;
		if (coordinateAlongNormal > max)
			max = coordinateAlongNormal;
	}
	if (min > max)
	{
		RaiseError("min should be less than max. But found min = "
			+ std::to_string(min) + " and max = " + std::to_string(max));
	}
	return { min, max };
}




