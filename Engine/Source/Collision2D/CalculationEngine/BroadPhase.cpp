#include "BroadPhase.h"

Shorts;
using Overlaps = vector<pair<Collider*, Collider*>>;
static Overlaps candidates; // candidates for overlap

Overlaps& BroadPhase::GetPotentialOverlaps()
{
	candidates.clear();
	auto& colliderIDs = Collider::CollidersSortedByX();
	for (int i = 0; i < (int)colliderIDs.size() - 1; i++)
	{
		auto& col1 = Entity::GetComponent<Collider>(colliderIDs[i]);
		if (!col1.IsFullyEnabled())
			continue;
		auto bounds1 = col1.Bare().GetBoundingBox();

		//vector<Collider*> candidatesX = { &col1 };
		for (int j = i + 1; j < (int)colliderIDs.size(); j++) // loop until no overlap along X
		{
			auto& col2 = Entity::GetComponent<Collider>(colliderIDs[j]);
			if (col1.GetTransform().IsStatic() && col2.GetTransform().IsStatic())
				continue;
			if (!col2.IsFullyEnabled())
				continue;
			auto bounds2 = col2.Bare().GetBoundingBox();

			if (!bounds1.IsOverlappingX(bounds2))
				break;
			//candidatesX.push_back(&col2);
			if (bounds1.IsOverlappingY(bounds2))
				candidates.emplace_back(&col1, &col2);
		}

		/* 
		std::sort(candidatesX.begin(), candidatesX.end(), SortByY());
		for (int j = i + 1; j < (int)candidatesX.size(); j++) // loop until no overlap along Y
		{
			auto& col2 = *candidatesX[j];
			if (!col2.IsFullyEnabled())
				continue;
			auto bounds2 = col2.Bare().GetBoundingBox();

			if (!bounds1.IsOverlappingY(bounds2))
				break;
			candidates.push_back({ &col1, &col2 });
		}
		*/
	}

	return candidates;
}

/*
struct SortByY
{
	bool operator() (Collider* lhs, Collider* rhs) const
	{
		return lhs->Get<Transform>().Position2D().y < rhs->Get<Transform>().Position2D().y;
	}
};
using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<double>; // units of seconds
vector<pair<const char*, time_point>> times;
inline time_point Now() { return std::chrono::high_resolution_clock::now(); }
inline void SetTime(const char* label) { times.push_back({ label, Now() }); }

void PrintAndResetTimes()
{
	if (times.empty())
		return;

	P("times:");
	double total = duration(times.back().second - times[0].second).count();
	for (int i = 1; i < (int)times.size(); i++)
	{
		double dur = duration(times[i].second - times[i - 1].second).count();
		P("    ", times[i].first, " | ", dur / total);
	}
	times.clear();
}
*/