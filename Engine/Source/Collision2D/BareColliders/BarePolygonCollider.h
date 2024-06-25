#pragma once
#include "BareCollider.h"
#include "glm/glm.hpp"


//class PolygonCollider;
// the polygon has to be convex. evt. introduce a check for this
class BarePolygonCollider : public BareCollider
{
public:
	Shorts;
	static BarePolygonCollider MakeRectangle(vec2 center, quat rot, vec2 size);
	static BarePolygonCollider Make(vector<vec2> positions);
	void Setup(ITransform iTransform_, vector<vec2> localPosition2Ds, bool isStatic);
	// add position and remove position would be useful in an editor

	const vector<vec2>& GetLocalPosition2Ds() const { return localPosition2Ds; }
	pair<float, float> ShadowAlongNormal(vec2 normal) const override;
	const vec2 GetNormalByIndex(int i) const { return iTransform.NonPosToWorldSpace(localNormals[i]); };
	vector<vec2> Positions() const override;
	//static bool Check(vector<vec2> localPosition2Ds_);
	void SetPosition(int index, vec2 newPosition); // used by inspector
	void AddPositionAfter(int priorPositionIndex); // used by inspector
	void RemovePosition(int index); // used by inspector
	BoundingBox GetBoundingBox() const override;

private:
	vector<vec2> localPosition2Ds; // positions in local space i.e. relative to transform.Position()
	vector<vec2> localNormals;
	vec2 centerOfMass; // same as average position2D, since we assume uniform density
	float maxExtension = 0;
	optional<BoundingBox> cachedBounds = std::nullopt;
	bool isStatic = false;

	void CalculateCaches(const vector<vec2>& localPosition2Ds_); // caches = cachedBounds, localNormals, centerOfMass and maxExtension
	void PruneEquivalentPositions(vector<vec2>& localPosition2Ds_);
	void CacheBounds();
};

