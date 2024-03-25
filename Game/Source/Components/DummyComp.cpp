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

	if (Input::KeyPressed(Keyboard::J))
	{
		logger::print("J: try destroy gamelogic from ", this->GetEntity());
		if (TryGet<GameLogic>())
			GetEntity().Destroy<GameLogic>();
	}
	if (Input::KeyPressed(Keyboard::H))
	{
		logger::print("H: try add gamelogic to ", this->GetEntity());
		if (!TryGet<GameLogic>() && TryGet<Renderable>() && TryGet<RectangleCollider>())
			GetEntity().Add<GameLogic>();
	}
	
	if (Input::KeyPressed(Keyboard::I))
	{
		logger::print("I: try destroy picture 1 RectangleCollider");
		if (Entity::GetEntity("picture 1").TryGet<RectangleCollider>())
			Entity::GetEntity("picture 1").Destroy<RectangleCollider>();
	}
	if (Input::KeyPressed(Keyboard::U))
	{
		logger::print("U: try destroy circle 1");
		if (Entity::TryGetID("circle 1"))
			Entity::GetEntity("circle 1").Destroy();
	}
	if (Input::KeyPressed(Keyboard::Y))
	{
		logger::print("Y: try make circle 1 and X");
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
			/*
			Entity& circle1 = Entity::register_.Add("circle 1");
			circle1.Add<Transform>();
			circle1.Add<CircleCollider>().SetLocalRadius(0.5f);
			*/
			Entity::Make<Transform, CircleCollider>("circle 1")
				.Get<CircleCollider>().SetLocalRadius(0.5);
		}
		if (!Entity::TryGetID("XX"))
			Entity::Make<DummyComp, DummyComp>("XX");
	}
	/*
	if (Input::KeyHeldDown(Keyboard::T))
	{
		logger::print("T: Save");
		Scene::Save();
	}
	*/
	if (Input::KeyPressed(Keyboard::R))
		logger::print("R: pressed");
	if (Input::KeyHeldDown(Keyboard::R))
		logger::print("R: held down");
	if (Input::KeyReleased(Keyboard::R))
		logger::print("R: released");

	if (Input::KeyPressed(Keyboard::ctrl))
		logger::print("ctrl: pressed");
	if (Input::KeyHeldDown(Keyboard::ctrl))
		logger::print("ctrl: held down");
	if (Input::KeyReleased(Keyboard::ctrl))
		logger::print("ctrl: released");
}
