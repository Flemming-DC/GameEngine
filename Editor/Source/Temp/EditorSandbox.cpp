#include "EditorSandbox.h"
#include "InputKey.h"
#include "ColQuery.h"
#include "Renderer.h"
#include "Time_.h"
#include "InputVectorizer.h"
#include "Screen.h"
#include "Camera.h"
#include "OpenGlSetup.h"
#include "SceneCamera.h"
#include "RectangleCollider.h"
#include "Renderable.h"
#include "Screen.h"
#include "InputVectorizer.h"
#include "OpenGlExternal.h"
#include "EngineAssets.h"


using namespace Editor;
using namespace Key;
Shorts;


void TestMousePosition()
{
    auto screenPos = InputVectorizer::GetVectorInput(Key::VectorKey::MouseEntireScreenPosition);
    //auto normScreenPos = InputVectorizer::GetVectorInput(Key::VectorKey::MouseEntireScreenPosition);
    //auto worldPos = InputVectorizer::GetVectorInput(Key::VectorKey::mouseWorldPosition2D);

    //auto back_to_normScreenPos = Screen::FromWorldPosition(glm::ToVec3(worldPos));


    auto editorWorldPos = Screen::ToWorldPosition(screenPos, vec2(674.0f, 146.0f), vec2(1634.599976f, 866.599976f),
        SceneCamera::Position(), SceneCamera::Rotation(), SceneCamera::Scale());
    if (InputKey::BecomesPressed(Keyboard::P))
    {
        logger::print("P");
        logger::print("screenPos: ", screenPos);
        //logger::print("normScreenPos: ", normScreenPos);
        //logger::print("worldPos: ", worldPos);
        //logger::print("back_to_normScreenPos: ", back_to_normScreenPos);
        logger::print("editorWorldPos: ", editorWorldPos);
    }
    if (InputKey::BecomesPressed(Keyboard::O))
        glfwSetWindowPos(OpenGlSetup::GetWindow(), 100, 100);
}

void TestDestruction()
{

	if (InputKey::BecomesPressed(Keyboard::L))
	{
		logger::print("L: try destroy Renderable from picture 1");
		Entity& p1 = Entity::GetEntity("picture 1");
		if (p1.TryGet<Renderable>())
			p1.Destroy<Renderable>();
	}
	if (InputKey::BecomesPressed(Keyboard::K))
	{
		logger::print("K: try add Renderable to picture 1");
		Entity& p1 = Entity::GetEntity("picture 1");
		if (!p1.TryGet<Renderable>())
			p1.Add<Renderable>();
	}

	if (InputKey::BecomesPressed(Keyboard::J))
	{
		logger::print("J: try destroy picture 1 RectangleCollider");
		if (Entity::GetEntity("picture 1").TryGet<RectangleCollider>())
			Entity::GetEntity("picture 1").Destroy<RectangleCollider>();
	}
	if (InputKey::BecomesPressed(Keyboard::H))
	{
		logger::print("H: try destroy circle 1");
		if (Entity::TryGetID("circle 1"))
			Entity::GetEntity("circle 1").Destroy();
	}
	if (InputKey::BecomesPressed(Keyboard::G))
	{
		logger::print("G: try create circle 1");
		if (!Entity::TryGetID("circle 1"))
			Entity::Make<Transform, CircleCollider>("circle 1").Get<CircleCollider>().Setup(0.5);
	}
}

void EditorSandbox::OnEditorStart()
{

}


void EditorSandbox::OnEditorUpdate()
{
    //TestMousePosition();
	TestDestruction();
}


