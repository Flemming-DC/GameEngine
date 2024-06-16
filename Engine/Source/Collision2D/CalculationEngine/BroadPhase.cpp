#include "BroadPhase.h"

Shorts;
using Overlaps = vector<pair<Collider*, Collider*>>;

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
	/*
	P("times:");
	double total = duration(times.back().second - times[0].second).count();
	for (int i = 1; i < (int)times.size(); i++)
	{
		double dur = duration(times[i].second - times[i - 1].second).count();
		P("    ", times[i].first, " | ", dur / total);
	}*/
	times.clear();
}


Overlaps FineTimedGetPotentialOverlaps()
{
	Overlaps candidates; // candidates for overlap
	auto colliderIDs = Collider::CollidersSortedByX();
	for (int i = 0; i < (int)colliderIDs.size() - 1; i++)
	{
		times.clear();
		SetTime("loop start         ");
		auto& col1 = Entity::GetComponent<Collider>(colliderIDs[i]);
		SetTime("col1 GetComponent  ");
		if (!col1.IsFullyEnabled())
			continue;
		SetTime("col1.IsFullyEnabled");
		auto bounds1 = col1.Bare().GetBoundingBox();
		SetTime("bounds1            ");

		//vector<Collider*> candidatesX = { &col1 };
		for (int j = i + 1; j < (int)colliderIDs.size(); j++) // loop until no overlap along X
		{
			auto& col2 = Entity::GetComponent<Collider>(colliderIDs[j]);
			if (!col2.IsFullyEnabled())
				continue;
			auto bounds2 = col2.Bare().GetBoundingBox();

			if (!bounds1.IsOverlappingX(bounds2))
				break;
			//candidatesX.push_back(&col2);
			if (bounds1.IsOverlappingY(bounds2))
				candidates.push_back({ &col1, &col2 });
		}
		SetTime("internal loop      ");
		PrintAndResetTimes();

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

	//if (candidates.size() > 5)
	//	P("potentialNewOverlapCount: ", candidates.size(), " out of ", Collider::CollidersSortedByX().size());

	return candidates;
}

Overlaps BroadPhase::GetPotentialOverlaps()
{
	Overlaps candidates; // candidates for overlap
	auto colliderIDs = Collider::CollidersSortedByX();
	for (int i = 0; i < (int)colliderIDs.size() - 1; i++)
	{
		auto& col1 = Entity::GetComponent<Collider>(colliderIDs[i]);
		if (!col1.IsFullyEnabled())
			continue;
		auto bounds1 = col1.Bare().GetBoundingBox();


		for (int j = i + 1; j < (int)colliderIDs.size(); j++) // loop until no overlap along X
		{
			auto& col2 = Entity::GetComponent<Collider>(colliderIDs[j]);
			if (!col2.IsFullyEnabled())
				continue;
			auto bounds2 = col2.Bare().GetBoundingBox();

			if (!bounds1.IsOverlappingX(bounds2))
				break;
			if (bounds1.IsOverlappingY(bounds2))
				candidates.push_back({ &col1, &col2 });
		}
		// this only applies sweep and prune along the x direction
	}

	return candidates;
}

void Nothing() {}
void DoNothingWithVeryLongName_BlahBlahBlahBlahBlahBlahBlahBlahBlahBlahBlahBlah() {}

Overlaps NewGetPotentialOverlaps() // BroadPhase::
{
	ProfileFunc;
	ProfileLine(Nothing());
	Overlaps candidates; // candidates for overlap
	auto colliderIDs = Collider::CollidersSortedByX();
	for (int i = 0; i < (int)colliderIDs.size() - 1; i++)
	{
		ProfileLine(DoNothingWithVeryLongName_BlahBlahBlahBlahBlahBlahBlahBlahBlahBlahBlahBlah());
		ProfileLine(Nothing());
		ProfileLine(auto& col1 = Entity::GetComponent<Collider>(colliderIDs[i]););
		ProfileLine(if (!col1.IsFullyEnabled())
			continue;);
		ProfileLine(auto bounds1 = col1.Bare().GetBoundingBox(););

		//vector<Collider*> candidatesX = { &col1 };
		for (int j = i + 1; j < (int)colliderIDs.size(); j++) // loop until no overlap along X
		{
			ProfileLine(auto& col2 = Entity::GetComponent<Collider>(colliderIDs[j]););
			ProfileLine(if (!col2.IsFullyEnabled())
				continue;);
			ProfileLine(auto bounds2 = col2.Bare().GetBoundingBox(););

			ProfileLine(if (!bounds1.IsOverlappingX(bounds2))
			{
				//if (j - i > 3)
				//	P("overlap along x count ", j - i);
				break;
			});
			//ProfileLine(candidatesX.push_back(&col2););
			ProfileLine(if (bounds1.IsOverlappingY(bounds2))
				candidates.push_back({ &col1, &col2 }););
		}

		/*
		ProfileLine(std::sort(candidatesX.begin(), candidatesX.end(), SortByY()););
		for (int j = i + 1; j < (int)candidatesX.size(); j++) // loop until no overlap along Y
		{
			ProfileLine(auto & col2 = *candidatesX[j];);
			ProfileLine(if (!col2.IsFullyEnabled())
				continue;);
			ProfileLine(auto bounds2 = col2.Bare().GetBoundingBox(););

			ProfileLine(if (!bounds1.IsOverlappingY(bounds2))
				break;);
			ProfileLine(candidates.push_back({ &col1, &col2 }););
		}*/
	}


	//if (candidates.size() > 3)
	//	P("potentialNewOverlapCount: ", candidates.size(), " out of ", Collider::CollidersSortedByX().size());

	return candidates;
}




Overlaps NewTimedGetPotentialOverlaps()
{
	ProfileFunc;
	Overlaps candidates; // candidates for overlap
	auto colliderIDs = Collider::CollidersSortedByX();
	for (int i = 0; i < (int)colliderIDs.size() - 1; i++)
	{
		ProfileLine(auto & col1 = Entity::GetComponent<Collider>(colliderIDs[i]););
		ProfileLine(if (!col1.IsFullyEnabled())
			continue;);
		ProfileLine(auto bounds1 = col1.Bare().GetBoundingBox(););

		vector<Collider*> candidatesX = { &col1 };
		for (int j = i + 1; j < (int)colliderIDs.size(); j++) // loop until no overlap along X
		{
			ProfileLine(auto & col2 = Entity::GetComponent<Collider>(colliderIDs[j]););
			ProfileLine(if (!col2.IsFullyEnabled())
				continue;);
			ProfileLine(auto bounds2 = col2.Bare().GetBoundingBox(););

			ProfileLine(if (!bounds1.IsOverlappingX(bounds2))
				break;);
			ProfileLine(candidatesX.push_back(&col2););
		}

		//Collider::CollidersSortedByY();
		//candidatesX
		//vector<Collider*> sortedCandidatesX;// = Collider::CollidersSortedByY() where col in candidatesX;
		// looks slow
		/*
		std::copy_if(Collider::CollidersSortedByY().begin(), Collider::CollidersSortedByY().end(),
			std::back_inserter(sortedCandidatesX), [](Collider* c)
			{
				return c % 2 == 0; // Condition: keep only even numbers
			});
		*/
		struct SortByY
		{
			bool operator() (Collider* lhs, Collider* rhs) const
			{
				return lhs->Get<Transform>().Position2D().y < rhs->Get<Transform>().Position2D().y;
			}
		};
		ProfileLine(std::sort(candidatesX.begin(), candidatesX.end(), SortByY()););

		for (int j = i + 1; j < (int)candidatesX.size(); j++) // loop until no overlap along Y
		{
			ProfileLine(auto & col2 = *candidatesX[j];);
			ProfileLine(if (!col2.IsFullyEnabled())
				continue;);
			ProfileLine(auto bounds2 = col2.Bare().GetBoundingBox(););

			ProfileLine(if (!bounds1.IsOverlappingY(bounds2))
				break;);
			ProfileLine(candidates.push_back({ &col1, &col2 }););
		}
	}


	//if (candidates.size() > 3)
	//	P("potentialNewOverlapCount: ", candidates.size(), " out of ", Collider::CollidersSortedByX().size());

	return candidates;
}
/*
Overlaps OldGetPotentialOverlaps()
{
	ProfileFunc;
	Overlaps candidates; // candidates for overlap

	ProfileLine(auto colliderIDs = Collider::GetAllColliders(););
	for (int i = 0; i < (int)colliderIDs.size(); i++)
	{
		ProfileLine(auto & col1 = Entity::GetComponent<Collider>(colliderIDs[i]););
		ProfileLine(if (!col1.IsFullyEnabled())
			continue;);
		ProfileLine(auto bounds1 = col1.Bare().GetBoundingBox(););

		for (int j = 0; j < i; j++)
		{
			ProfileLine(auto & col2 = Entity::GetComponent<Collider>(colliderIDs[j]););
			ProfileLine(if (!col2.IsFullyEnabled())
				continue;);
			ProfileLine(auto bounds2 = col2.Bare().GetBoundingBox(););

			ProfileLine(if (!bounds1.IsOverlapping(bounds2))
				continue;);
			ProfileLine(candidates.push_back({ &col1, &col2 }););
		}
	}

	if (candidates.size() > 3)
		P("potentialNewOverlapCount: ", candidates.size(), " out of ", Collider::CollidersSortedByX().size());

	return candidates;
}
*/

