#include "Component.h"


namespace Editor
{

	class Inspector
	{
	public:
		static void Start();
		static void Update();

	private:
		static void DrawComponent(const Component& comp);
	};
}





