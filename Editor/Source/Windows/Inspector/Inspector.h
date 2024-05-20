#include "Component.h"


namespace Editor
{
	class Inspector
	{
	public:
		static void Start();
		static void Update();

	private:
		static void DrawEntityHeader();
		static void DrawComponent(Component& comp);
		static void AttachComponent();

	};
}





