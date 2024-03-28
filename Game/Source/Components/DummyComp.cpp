#include "DummyComp.h"
#include "Input.h"
#include "GameLogic.h"
#include "RectangleCollider.h"
#include "Screen.h"
#include "InputVectorizer.h"
#include "GameInputs.h"


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
	/*
	for (const auto& g : Input::GamepadIDs())
		logger::print("DummyComp.OnStart: connected at startup ", g);
	onConnectedChangedIndex = Input::onGamepadConnectionChanged.Add([](bool hasBecomeConnected, unsigned int gamepadID)
		{
			logger::print("DummyComp.OnStart.onGamepadConnectionChanged: ", hasBecomeConnected, " ", gamepadID);
		});
	*/
}

void DummyComp::OnDestroy()
{
	Input::onGamepadConnectionChanged.Remove(onConnectedChangedIndex);
}

void DummyComp::OnUpdate()
{
	if (Input::IsPressed(Keyboard::L))
	{
		logger::print("L");
		logger::print(GameInputs::jump.State());
		logger::print(GameInputs::jump.Delta());
		logger::print(GameInputs::jump.ActivationDuration());
	}
	if (GameInputs::jump.IsPressed())
		logger::print("jump IsPressed");
	if (GameInputs::jump.IsReleased())
		logger::print("jump IsReleased");


	if (Input::IsPressed(Keyboard::K))
	{
		logger::print("K");
		logger::print(GameInputs::fire.State());
		logger::print(GameInputs::fire.Delta());
		logger::print(GameInputs::fire.ActivationDuration());
	}
	if (GameInputs::fire.IsPressed())
		logger::print("fire IsPressed");
	if (GameInputs::fire.IsReleased())
		logger::print("fire IsReleased");

	if (Input::IsPressed(Keyboard::J))
	{
		logger::print("J");
		logger::print(GameInputs::walk.State());
		logger::print(GameInputs::walk.Delta());
		logger::print(GameInputs::walk.ActivationDuration());
	}
	if (GameInputs::walk.IsPressed())
		logger::print("walk IsPressed");
	if (GameInputs::walk.IsReleased())
		logger::print("walk IsReleased");


}


void DummyComp::OldTests()
{

	if (Input::IsPressed(Keyboard::J))
	{
		logger::print("J: try destroy gamelogic from ", this->GetEntity());
		if (TryGet<GameLogic>())
			GetEntity().Destroy<GameLogic>();
	}
	if (Input::IsPressed(Keyboard::H))
	{
		logger::print("H: try add gamelogic to ", this->GetEntity());
		if (!TryGet<GameLogic>() && TryGet<Renderable>() && TryGet<RectangleCollider>())
			GetEntity().Add<GameLogic>();
	}

	if (Input::IsPressed(Keyboard::I))
	{
		logger::print("I: try destroy picture 1 RectangleCollider");
		if (Entity::GetEntity("picture 1").TryGet<RectangleCollider>())
			Entity::GetEntity("picture 1").Destroy<RectangleCollider>();
	}
	if (Input::IsPressed(Keyboard::U))
	{
		logger::print("U: try destroy circle 1");
		if (Entity::TryGetID("circle 1"))
			Entity::GetEntity("circle 1").Destroy();
	}
	if (Input::IsPressed(Keyboard::Y))
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
	if (Input::IsPressed(Keyboard::R))
		logger::print("R: pressed");
	if (Input::IsHeldDown(Keyboard::R))
		logger::print("R: held down");
	if (Input::IsReleased(Keyboard::R))
		logger::print("R: released");

	if (Input::IsPressed(Keyboard::ctrl))
		logger::print("ctrl: pressed");
	if (Input::IsHeldDown(Keyboard::ctrl))
		logger::print("ctrl: held down");
	if (Input::IsReleased(Keyboard::ctrl))
		logger::print("ctrl: released");



	if (Input::IsPressed(Mouse::middle))
		logger::print("Mouse::middle: pressed");
	if (Input::IsHeldDown(Mouse::middle))
		logger::print("Mouse::middle: held down");
	if (Input::IsReleased(Mouse::middle))
		logger::print("Mouse::middle: released");

	if (Input::IsPressed(Keyboard::G))
		logger::print("G: ", Input::MouseScreenPosition());
	if (Input::IsPressed(Keyboard::F))
		logger::print("F: ", Input::MouseWorldPosition2D());

	if (Input::IsPressed(Keyboard::B))
	{
		bool foundDepth;
		auto pos = Screen::ToWorldPosition(Input::MouseScreenPosition(), &foundDepth);
		logger::print("B: ", foundDepth, " ", pos);
	}

	if (Input::IsPressed(Keyboard::_1))
	{
		auto before = Input::MouseScreenPosition();
		auto world = Screen::ToWorldPosition(before);
		auto after = Screen::FromWorldPosition(world);
		logger::print("1: ", before, " == ", after);
	}

	if (Input::IsPressed(Keyboard::_2))
	{
		logger::print("2: ");
		Screen::SetCursorMode(CursorMode::normal);
	}
	if (Input::IsHeldDown(Keyboard::_3))
	{
		logger::print("3: ");
		Screen::SetCursorMode(CursorMode::hidden);
	}
	if (Input::IsPressed(Keyboard::_4))
	{
		logger::print("4: ");
		Screen::SetCursorMode(CursorMode::locked);
	}

	if (Input::IsPressed(Keyboard::_5))
	{
		logger::print("5: ");
		logger::print(Input::HasGamepad(GLFW_JOYSTICK_2));
	}
	if (Input::IsPressed(Keyboard::_6))
	{
		logger::print("6: ");
		logger::print(Input::GetFloat(FloatKey::leftStick_x, GLFW_JOYSTICK_2));
	}


	if (Input::IsPressed(Gamepad::A, GLFW_JOYSTICK_2))
	{
		logger::print("Gamepad::A: IsPressed", GLFW_JOYSTICK_2);
	}
	if (Input::IsHeldDown(Gamepad::A, GLFW_JOYSTICK_2))
	{
		logger::print("Gamepad::A: IsHeldDown", GLFW_JOYSTICK_2);
	}
	if (Input::IsReleased(Gamepad::A, GLFW_JOYSTICK_2))
	{
		logger::print("Gamepad::A: IsReleased ", GLFW_JOYSTICK_2);
	}

	if (Input::IsPressed(Keyboard::_7))
	{
		glm::vec2 vec = InputVectorizer::GetVectorInput(VectorKey::rightStick, GLFW_JOYSTICK_2);
		logger::print("7: joystickRight ", vec);
	}
	if (Input::IsPressed(Keyboard::_8))
	{
		logger::print("8: ");
		logger::print(Input::GetFloat(FloatKey::leftStick_y));
	}
	if (Input::IsPressed(Keyboard::_9))
	{
		logger::print("9: ");
		logger::print(Input::GetFloat(FloatKey::leftTrigger));
	}
}