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

void DummyComp::OnDestroy()
{

	Gizmo::register_.Remove(gizmoID);
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
	
	if (Input::KeyHeldDown(Keyboard::I))
	{
		logger::print("I");
		Entity::GetEntity("picture 1").Destroy();
	}
	if (Input::KeyHeldDown(Keyboard::U))
	{
		logger::print("U");
		if (Entity::TryGetID("circle 1"))
			Entity::GetEntity("circle 1").Destroy();
	}
	if (Input::KeyHeldDown(Keyboard::Y))
	{
		logger::print("Y");
		if (!Entity::TryGetID("circle 1"))
		{
			/*
			Delay::ToFrameEnd([]()
			{
				Entity& circle1 = Entity::register_.Add("circle 1");
				circle1.Add<Transform>();
				circle1.Add<CircleCollider>().SetLocalRadius(0.5f);
			});
			*/
			// tilsyneladende kan man sagtens lave entiteter midt i et update loop
			Entity& circle1 = Entity::register_.Add("circle 1");
			circle1.Add<Transform>();
			circle1.Add<CircleCollider>().SetLocalRadius(0.5f);
		}
	}

	if (Input::KeyHeldDown(Keyboard::T))
	{
		logger::print("T");
		Scene::Save();
	}

}
