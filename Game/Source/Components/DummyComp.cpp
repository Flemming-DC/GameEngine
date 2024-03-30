#include "DummyComp.h"
#include "InputKey.h"
#include "GameLogic.h"
#include "RectangleCollider.h"
#include "Screen.h"
#include "InputVectorizer.h"
#include "GameInputs.h"
#include "OpenGlExternal.h"

using namespace Key;

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

	gizmoID = Gizmo::Make(position2Ds, &GetTransform());
	/*
	for (const auto& g : InputKey::GamepadIDs())
		logger::print("DummyComp.OnStart: connected at startup ", g);
	onConnectedChangedIndex = InputKey::onGamepadConnectionChanged.Add([](bool hasBecomeConnected, unsigned int gamepadID)
		{
			logger::print("DummyComp.OnStart.onGamepadConnectionChanged: ", hasBecomeConnected, " ", gamepadID);
		});
	*/
}

void DummyComp::OnDestroy()
{
	InputKey::onGamepadConnectionChanged.Remove(onConnectedChangedIndex);
}

void DummyComp::OnUpdate()
{
	TestInputActions();
}


void DummyComp::TestInputActions()
{

	if (InputKey::BecomesPressed(Keyboard::L))
	{
		logger::print("L");
		logger::print(GameInputs::Jump().State());
		logger::print(GameInputs::Jump().Delta());
		logger::print(GameInputs::Jump().StateDuration());
	}
	if (GameInputs::Jump().BecomesPressed())
		logger::print("jump BecomesPressed");
	if (GameInputs::Jump().BecomesReleased())
		logger::print("jump BecomesReleased");


	if (InputKey::BecomesPressed(Keyboard::K))
	{
		logger::print("K");
		logger::print(GameInputs::Fire().State());
		logger::print(GameInputs::Fire().Delta());
		logger::print(GameInputs::Fire().StateDuration());
	}
	if (GameInputs::Fire().BecomesPressed())
		logger::print("fire BecomesPressed");
	if (GameInputs::Fire().BecomesReleased())
		logger::print("fire BecomesReleased");

	if (InputKey::BecomesPressed(Keyboard::J))
	{
		logger::print("J");
		logger::print(GameInputs::Walk().State());
		logger::print(GameInputs::Walk().Delta());
		logger::print(GameInputs::Walk().StateDuration());
	}
	if (GameInputs::Walk().BecomesPressed())
		logger::print("walk BecomesPressed");
	if (GameInputs::Walk().BecomesReleased())
		logger::print("walk BecomesReleased");


	if (InputKey::BecomesPressed(Keyboard::H))
	{
		logger::print("H");
		GameInputs::Walk().enabled = !GameInputs::Walk().enabled;
	}
}

void DummyComp::OldTests()
{

	if (InputKey::BecomesPressed(Keyboard::J))
	{
		logger::print("J: try destroy gamelogic from ", this->GetEntity());
		if (TryGet<GameLogic>())
			GetEntity().Destroy<GameLogic>();
	}
	if (InputKey::BecomesPressed(Keyboard::H))
	{
		logger::print("H: try add gamelogic to ", this->GetEntity());
		if (!TryGet<GameLogic>() && TryGet<Renderable>() && TryGet<RectangleCollider>())
			GetEntity().Add<GameLogic>();
	}

	if (InputKey::BecomesPressed(Keyboard::I))
	{
		logger::print("I: try destroy picture 1 RectangleCollider");
		if (Entity::GetEntity("picture 1").TryGet<RectangleCollider>())
			Entity::GetEntity("picture 1").Destroy<RectangleCollider>();
	}
	if (InputKey::BecomesPressed(Keyboard::U))
	{
		logger::print("U: try destroy circle 1");
		if (Entity::TryGetID("circle 1"))
			Entity::GetEntity("circle 1").Destroy();
	}
	if (InputKey::BecomesPressed(Keyboard::Y))
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
	if (InputKey::KeyHeldDown(Keyboard::T))
	{
		logger::print("T: Save");
		Scene::Save();
	}
	*/
	if (InputKey::BecomesPressed(Keyboard::R))
		logger::print("R: pressed");
	if (InputKey::IsPressed(Keyboard::R))
		logger::print("R: held down");
	if (InputKey::BecomesReleased(Keyboard::R))
		logger::print("R: released");

	if (InputKey::BecomesPressed(Keyboard::ctrl))
		logger::print("ctrl: pressed");
	if (InputKey::IsPressed(Keyboard::ctrl))
		logger::print("ctrl: held down");
	if (InputKey::BecomesReleased(Keyboard::ctrl))
		logger::print("ctrl: released");



	if (InputKey::BecomesPressed(Mouse::middle))
		logger::print("Mouse::middle: pressed");
	if (InputKey::IsPressed(Mouse::middle))
		logger::print("Mouse::middle: held down");
	if (InputKey::BecomesReleased(Mouse::middle))
		logger::print("Mouse::middle: released");

	if (InputKey::BecomesPressed(Keyboard::G))
		logger::print("G: ", InputKey::MouseScreenPosition());
	if (InputKey::BecomesPressed(Keyboard::F))
		logger::print("F: ", InputKey::MouseWorldPosition2D());

	if (InputKey::BecomesPressed(Keyboard::B))
	{
		bool foundDepth;
		auto pos = Screen::ToWorldPosition(InputKey::MouseScreenPosition(), &foundDepth);
		logger::print("B: ", foundDepth, " ", pos);
	}

	if (InputKey::BecomesPressed(Keyboard::_1))
	{
		auto before = InputKey::MouseScreenPosition();
		auto world = Screen::ToWorldPosition(before);
		auto after = Screen::FromWorldPosition(world);
		logger::print("1: ", before, " == ", after);
	}

	if (InputKey::BecomesPressed(Keyboard::_2))
	{
		logger::print("2: ");
		Screen::SetCursorMode(CursorMode::normal);
	}
	if (InputKey::IsPressed(Keyboard::_3))
	{
		logger::print("3: ");
		Screen::SetCursorMode(CursorMode::hidden);
	}
	if (InputKey::BecomesPressed(Keyboard::_4))
	{
		logger::print("4: ");
		Screen::SetCursorMode(CursorMode::locked);
	}

	if (InputKey::BecomesPressed(Keyboard::_5))
	{
		logger::print("5: ");
		logger::print(InputKey::HasGamepad(GLFW_JOYSTICK_2));
	}
	if (InputKey::BecomesPressed(Keyboard::_6))
	{
		logger::print("6: ");
		logger::print(InputKey::GetFloat(FloatKey::leftStick_x, GLFW_JOYSTICK_2));
	}


	if (InputKey::BecomesPressed(Gamepad::A, GLFW_JOYSTICK_2))
	{
		logger::print("Gamepad::A: BecomesPressed", GLFW_JOYSTICK_2);
	}
	if (InputKey::IsPressed(Gamepad::A, GLFW_JOYSTICK_2))
	{
		logger::print("Gamepad::A: IsPressed", GLFW_JOYSTICK_2);
	}
	if (InputKey::BecomesReleased(Gamepad::A, GLFW_JOYSTICK_2))
	{
		logger::print("Gamepad::A: BecomesReleased ", GLFW_JOYSTICK_2);
	}

	if (InputKey::BecomesPressed(Keyboard::_7))
	{
		glm::vec2 vec = InputVectorizer::GetVectorInput(VectorKey::rightStick, GLFW_JOYSTICK_2);
		logger::print("7: joystickRight ", vec);
	}
	if (InputKey::BecomesPressed(Keyboard::_8))
	{
		logger::print("8: ");
		logger::print(InputKey::GetFloat(FloatKey::leftStick_y));
	}
	if (InputKey::BecomesPressed(Keyboard::_9))
	{
		logger::print("9: ");
		logger::print(InputKey::GetFloat(FloatKey::leftTrigger));
	}
}