#include "Core.h"
#include "Dynamic.h"
#include "GameAssets.h"
#include "Editor.h" 
#include "DemoScene.h"
#include "SecondScene.h"
#include "MiniScene.h"
#include "ShortHands.h"
#include <type_traits>

//Scene::MakeBlankSceneFile("SecondScene");
//Scene::GetActiveScene().PurelyManualSetup();
//Scene::GetActiveScene().Save();

// SFINAE, enable_if example
// based on http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence


#include <iostream>
#include <type_traits>

class ClassWithToString
{
public:
    std::string to_string() { return "description "; }
};

class ClassNoToString
{
public:
    int a;
};


// SFINAE test
template <typename T>
class HasToString
{
private:
    typedef char YesType[1];
    typedef char NoType[2];

    template <typename T2> static YesType& test(decltype(&T2::to_string));
    template <typename T2> static NoType& test(...);


public:
    enum { answer = (sizeof(test<T>(0)) == sizeof(YesType)) };
};

template<typename T>
typename std::enable_if<HasToString<T>::answer, std::string>::type
CallToString(T* t)
{
    return t->to_string();
}


std::string CallToString(...)
{
    return "no to_string() method";
}


int _main()
{
    P(1);
    std::cout << HasToString<ClassWithToString>::answer << std::endl;
    std::cout << HasToString<ClassNoToString>::answer << std::endl;
    std::cout << HasToString<int>::answer << std::endl;
    P(2);

    ClassWithToString c1;
    std::cout << CallToString(&c1) << std::endl;
    P(3);

    ClassNoToString c2;
    std::cout << CallToString(&c2) << std::endl;
    P(4);

    int c3 = 0;
    std::cout << CallToString(&c3) << std::endl;
    P(5);

    return 0;
}

int main()
{
    //_main();
    //logger::print(UuidCreator::MakeID());
    Dynamic::Setup<Editor, GameAssets>();
    Core::Run<DemoScene>();
    return 0;
}
// findstr /S /M "_Debug" "Engine\Source\*.*"
// 
// git add . && git commit -m "m" && git push

