#include "Transform.h"

namespace Editor
{
	class Hierarchy
	{
	public:
		Shorts;
		static void Update();

	private:
		static vector<Transform*> FindRoots(); // if stored across frames, then switch to uuid
		static void DrawTreeNode(const Transform& transform);
		static void DrawTreeNode_old(const Transform& transform);
	};

}