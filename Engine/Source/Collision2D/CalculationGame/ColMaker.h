#include "BareCircleCollider.h"
#include "BarePolygonCollider.h"

class ColMaker
{
public:
	Shorts;
	inline static BareCircleCollider Circle(vec2 center, float radius) { return BareCircleCollider::Make(center, radius); }
	inline static BareCircleCollider Point(vec2 position) { return BareCircleCollider::MakePoint(position); }
	inline static BarePolygonCollider Polygon(vector<vec2> positions) { return BarePolygonCollider::Make(positions); }
	
	inline static BarePolygonCollider Rectangle(vec2 center, quat rot, vec2 size) 
		{ return BarePolygonCollider::MakeRectangle(center, rot, size); }
	inline static BarePolygonCollider Rectangle(vec2 center, vec2 size) 
		{ return BarePolygonCollider::MakeRectangle(center, glm::NoRotation(), size); }
	// make Ray / LineSegment
};



