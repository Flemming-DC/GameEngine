#pragma once


namespace Editor
{
	class SceneVisuals
	{
	public:
		static void Update();
	private:

		static void DrawGrid();
		static void DrawColliders();
	};

}