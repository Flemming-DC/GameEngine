#include "DummyComp.h"
#include "Input.h"
#include "GameLogic.h"
#include "RectangleCollider.h"



void DummyComp::OnStart()
{
	glm::vec2 halfSize = glm::vec2(0.5f, 0.5f);
	std::vector<glm::vec2> position2Ds =
	{
		{-halfSize.x, -halfSize.y }, // LD
		{-halfSize.x,  halfSize.y }, // LU
		{ halfSize.x,  halfSize.y }, // RU
		{ halfSize.x, -halfSize.y }, // RD
	};

	gizmoID = Gizmo::register_.Add(position2Ds, &GetTransform()).GetID();

}

void DummyComp::OnUpdate()
{
	if (Input::KeyHeldDown(Keyboard::J))
	{
		logger::print("J");
		GetEntity().Destroy<GameLogic>();
	}
	if (Input::KeyHeldDown(Keyboard::H))
	{
		logger::print("H");
		if (!TryGet<GameLogic>())
			GetEntity().Add<GameLogic>();
	}

}
