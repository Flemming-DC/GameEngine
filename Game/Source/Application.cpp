#include "Core.h"
#include "Dynamic.h"
#include "GameAssets.h"
#include "Editor.h" 
#include "DemoScene.h"
#include "SecondScene.h"
#include "MiniScene.h"
#include "GameInputs.h"

//Scene::MakeBlankSceneFile("SecondScene");
//Scene::GetActiveScene().PurelyManualSetup();
//Scene::GetActiveScene().Save();

#include <iostream>

template <typename... >
using void_t = void;

template <typename T, typename = void>
struct is_const_callable_method : std::false_type {};

template <typename T>
struct is_const_callable_method<T, void_t<
    decltype(std::declval<const T&>().to_string())
    >> : std::true_type {};


int _main()
{
    std::cout << is_const_callable_method<InputAction<bool>>::value << std::endl;
    std::cout << is_const_callable_method<Entity>::value << std::endl;
    return 0;
}

int main()
{
    _main();
    //logger::print(UuidCreator::MakeID());
    Dynamic::Setup<Editor, GameAssets, GameInputs>();
    Core::Run<DemoScene>();
    return 0;
}
// findstr /S /M "_Debug" "Engine\Source\*.*"
// 
// git add . && git commit -m "m" && git push

