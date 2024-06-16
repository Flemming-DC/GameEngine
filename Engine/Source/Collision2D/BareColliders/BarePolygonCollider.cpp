#include "BarePolygonCollider.h"
#include "ErrorChecker.h"
#include "GlmCheck.h"

static const inline float minPositionSeparation = glm::pow(10.0f, -5.0f);

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
	PruneEquivalentPositions(localPosition2Ds_);
	iTransform = iTransform_,
	localPosition2Ds = localPosition2Ds_;
	CalculateNormalsAndCenterOfMass(localPosition2Ds_);
}

void BarePolygonCollider::CalculateNormalsAndCenterOfMass(const vector<vec2>& localPosition2Ds_)
{
	// calculate additional data
	localNormals.clear();
	int count = (int)localPosition2Ds.size();
	for (int i = 0; i < count; i++)
	{
		int nextIndex = i + 1 < count ? i + 1 : 0;
		auto vectorAlongEdge = localPosition2Ds[nextIndex] - localPosition2Ds[i];
		/*
		if (glm::LessThan(vectorAlongEdge, minPositionSeparation))
		{
			// evt. replace with a warning and a hacky fix
			RaiseError("localPosition2Ds contains the elements ", localPosition2Ds[nextIndex], ", ", localPosition2Ds[i],
				", which yields vectorAlongEdge from positionIndex", i, " to positionIndex", nextIndex, " with a length of ",
				glm::Magnitude(vectorAlongEdge), ", which is too small.");
		}
		*/
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
		glm::vec2 radialvector = Check_vec(glm::normalize(localPosition2Ds[i] - centerOfMass));
		divergence += glm::dot(localNormals[i], radialvector);
	}
	if (divergence < 0)
	{
		for (int i = 0; i < count; i++)
			localNormals[i] *= -1;
	}
	Deny(glm::isnan(divergence),
		"Divergence is ", divergence, " on ", Tools::TypeName(*this), ".\nlocalPosition2Ds = ", localPosition2Ds);
}


std::pair<float, float> BarePolygonCollider::ShadowAlongNormal(vec2 normal) const
{
	Deny(glm::HasNAN(normal),
		"normal is nan: ", normal);
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
	InDebug(
		if (min > max)
		{
			float min_ = INFINITY;
			float max_ = -INFINITY;
			for (vec2 localPosition2D : localPosition2Ds)
			{
				//glm::vec2 position2D = isLocal ? localPosition2D : GetTransform().ToWorldSpace(localPosition2D, true);
				glm::vec2 position2D = iTransform.ToWorldSpace(localPosition2D, true);
				float coordinateAlongNormal = glm::dot(position2D, normal);
				if (coordinateAlongNormal < min_)
					min_ = coordinateAlongNormal;
				if (coordinateAlongNormal > max_)
					max_ = coordinateAlongNormal;
			}

			RaiseError(Tools::TypeName(*this) + " min should be less than max. But found min = ", min, " and max = ", max,
				"\n localPosition2Ds: ", localPosition2Ds, "\n normal:", normal);
		}
	);
	return { min, max };
}

std::vector<glm::vec2> BarePolygonCollider::Positions() const
{
	vector<vec2> positions;
	for (auto localPosition2D : localPosition2Ds)
		positions.push_back(iTransform.ToWorldSpace(localPosition2D, true));
	return positions;
}


void BarePolygonCollider::PruneEquivalentPositions(vector<vec2>& localPosition2Ds_)
{
	// try to find a pair of equivalent positions
	int indexToEliminate = -1;
	int count = (int)localPosition2Ds_.size();
	for (int i = 0; i < count; i++)
	{
		int nextIndex = i + 1 < count ? i + 1 : 0;
		auto vectorAlongEdge = localPosition2Ds_[nextIndex] - localPosition2Ds_[i];
		if (glm::LessThan(vectorAlongEdge, minPositionSeparation))
		{
			indexToEliminate = nextIndex;
			break;
		}
	}
	bool found = (indexToEliminate != -1);
	if (found)
	{
		Warning("Eliminating position ", localPosition2Ds_[indexToEliminate], " in ", Tools::TypeName(*this),
			", since it is too close to another position.\n",
			"localPosition2Ds = ", localPosition2Ds_);
		Tools::RemoveIndex(localPosition2Ds_, indexToEliminate);
		PruneEquivalentPositions(localPosition2Ds_);
	}
	else
	{ 
		InDebug(
			if (localPosition2Ds_.size() < 3)
				RaiseError("localPosition2Ds.size() is below 3. This is not a valid " + Tools::TypeName(*this));

			// find max and min coordinates so as to check if the points are one a line
			float minX = +INFINITY;
			float minY = +INFINITY;
			float maxX = -INFINITY;
			float maxY = -INFINITY;
			for (const vec2& pos : localPosition2Ds_)
			{
				if (pos.x < minX)
					minX = pos.x;
				if (pos.x > maxX)
					maxX = pos.x;

				if (pos.y < minY)
					minY = pos.y;
				if (pos.y > maxY)
					maxY = pos.y;
			}
			float width = std::abs(maxX - minX);
			float height = std::abs(maxY - minY);
			if (Check_p(width) < minPositionSeparation || Check_p(height) < minPositionSeparation)
				RaiseError("The points are all on a single line. This is not a valid polygon.");
		); 
	}
}


//void BarePolygonCollider::CheckConvexity(vector<vec2>& localPosition2Ds_)

void BarePolygonCollider::SetPosition(int index, vec2 newPosition)
{
	// check if points are too close or if collider is becoming non-convex or non-simple
	int count = localPosition2Ds.size();
	for (int i = 0; i < count; i++)
	{
		if (i == index)
			continue;
		auto lineToOldPosition = localPosition2Ds[index] - localPosition2Ds[i];
		auto lineToNewPosition = newPosition - localPosition2Ds[i];

		if (glm::Magnitude(lineToNewPosition) < GlmCheck::realisticallySmall)
		{
			Warning("Point nr ", index, " and ", i, " are getting too close.");
			return;
		}

		for (int j = i + 1; j < count; j++)
		{
			if (j == index)
				continue;
			// this line can either be an edge or an internal line
			vec2 line = localPosition2Ds[j] - localPosition2Ds[i];
			float crossProductOld = lineToOldPosition.x * line.y - lineToOldPosition.y * line.x;
			float crossProductNew = lineToNewPosition.x * line.y - lineToNewPosition.y * line.x;
			
			// check if the point crosses the line
			if (crossProductOld >= 0 != crossProductNew >= 0)
			{
				Warning(
					"Point would be crossing line, if updated. Therefore update is cancelled "
					"in order to prevent non-convex and non-simple polygons.",
					"\nnewPosition = ", newPosition, 
					"\nline goes from", localPosition2Ds[i], " to ", localPosition2Ds[j]);
				return;
			}
		}


	}

	localPosition2Ds[index] = newPosition;
	CalculateNormalsAndCenterOfMass(localPosition2Ds);
}

void BarePolygonCollider::AddPositionAfter(int priorPositionIndex)
{
	int count = localPosition2Ds.size();
	int nextIndex = priorPositionIndex + 1 % count;
	vec2 priorPos = localPosition2Ds[priorPositionIndex];
	vec2 nextPos = localPosition2Ds[nextIndex];
	vec2 pos = (nextPos + priorPos) / 2.0f;
	localPosition2Ds.insert(localPosition2Ds.begin() + nextIndex, pos);
	CalculateNormalsAndCenterOfMass(localPosition2Ds);
}

void BarePolygonCollider::RemovePosition(int index)
{
	Deny(localPosition2Ds.size() <= 3,
		"A polygon must have at least 3 corners");
	Tools::RemoveIndex(localPosition2Ds, index);
	CalculateNormalsAndCenterOfMass(localPosition2Ds);
}

BoundingBox BarePolygonCollider::GetBoundingBox() const
{
	float minX = INFINITY;
	float minY = INFINITY;
	float maxX = -INFINITY; 
	float maxY = -INFINITY;
	for (const vec2& pos : Positions())
	{
		if (pos.x < minX)
			minX = pos.x;
		if (pos.y < minY)
			minY = pos.y;
		if (pos.x > maxX)
			maxX = pos.x;
		if (pos.y > maxY)
			maxY = pos.y;
	}
	return { minX, minY, maxX, maxY };
}





