#include "Component.h"


namespace Editor
{
	class Inspector
	{
	public:
		static void Start();
		static void Update();

	private:
		static void EntityHeader();
		static void DrawComponent(Component& comp);
		static void DrawAddComponent();

		static bool ComponentHeader(Component& comp);
	};
}





