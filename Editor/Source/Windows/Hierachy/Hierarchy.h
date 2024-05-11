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
		static void DropToRoot();
		static void DrawTreeNode(Transform& transform);
		static bool DragDrop(Transform& transform); // used to edit parenthood

	};

}