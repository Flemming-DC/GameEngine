#include "BareCircleCollider.h"
#include "BarePolygonCollider.h"

class ColMaker
{
public:
	Shorts;
	inline static BareCircleCollider MakeCircle(vec2 center, float radius) { return BareCircleCollider::Make(center, radius); }
	inline static BareCircleCollider MakePoint(vec2 position) { return BareCircleCollider::MakePoint(position); }
	inline static BarePolygonCollider MakePolygon(vector<vec2> positions) { return BarePolygonCollider::Make(positions); }
	inline static BarePolygonCollider MakeRectangle(vec2 center, quat rot, vec2 size) { return BarePolygonCollider::MakeRectangle(center, rot, size); }
	// make Ray / LineSegment
};



