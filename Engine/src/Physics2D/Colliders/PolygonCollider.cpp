#include "PolygonCollider.h"
#include "ErrorChecker.h"


void PolygonCollider::Setup(std::vector<glm::vec2> position2Ds_)
{
	localPosition2Ds = position2Ds_;

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
	gizmo = Gizmo(position2Ds_, GetTransform());
	/*
	if (gizmo != nullptr)
		delete gizmo;
	gizmo = new Gizmo(position2Ds_, GetTransform());
	*/
	
}




std::pair<float, float> PolygonCollider::ShadowAlongNormal(glm::vec2 normal) const
{
	float min = INFINITY;
	float max = -INFINITY;
	for (auto localPosition2D : localPosition2Ds)
	{
		//glm::vec2 position2D = isLocal ? localPosition2D : GetTransform()->ToWorldSpace(localPosition2D, true);
		glm::vec2 position2D = GetTransform()->ToWorldSpace(localPosition2D, true);
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





