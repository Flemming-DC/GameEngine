#pragma once
#include "Transform.h"
#include "Camera.h" 
#include "Renderable.h" 
#include "PolygonCollider.h" 
#include "RectangleCollider.h" 
#include "CircleCollider.h" 


namespace Editor
{
	class ComponentDrawer
	{
	public:
		static void DrawTransform(Transform& transform);
		static void DrawRenderable(Renderable& renderable);
		static void DrawPolygonCollider(PolygonCollider& poly);
		static void DrawRectangleCollider(RectangleCollider& rect);
		static void DrawCircleCollider(CircleCollider& circle);

	};
}


