Engine = project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "**.h", "**.cpp", "**.hpp", "**.inl" } 
    
    includedirs {
        "../Engine/Source/**",
        "../Engine/Dependencies/SrcDependencies/stduuid",
        "../Engine/Dependencies/SrcDependencies",
        "../Engine/Dependencies/BinDependencies/OpenGL/include",
        "../Engine/Dependencies/BinDependencies/yaml-cpp-master/include",
        "../Engine/Dependencies/BinDependencies/GLEW/include"
    }

    links {
        "glew32s",
        "glfw3", 
        "Opengl32",
    }
    ignoredefaultlibraries { "LIBCMT" }

    libdirs { -- evt. copy all libs to one folder and grab them from there
        "../Engine/Dependencies/BinDependencies/OpenGL/lib-vc2022",
        "../Engine/Dependencies/BinDependencies/yaml-cpp-master/build/Debug",
        "../Engine/Dependencies/BinDependencies/yaml-cpp-master/build/Release",
        "../Engine/Dependencies/BinDependencies/GLEW/lib/Release/Win32"
    }


    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { }
    filter { "action:vs*" }
        buildoptions { "/wd26812" } -- Disable warning C26812 (unscoped enum) in Visual Studio

    filter "configurations:Debug"
        defines { "GLEW_STATIC", "WIN32", "_DEBUG", "_CONSOLE" } 
        runtime "Debug"
        symbols "On"
        links { "yaml-cppd" } -- append yaml-cppd library for Windows

    filter "configurations:Release"
        defines { "GLEW_STATIC", "WIN32", "NDEBUG", "_CONSOLE" } 
        runtime "Release"
        optimize "On" 
        symbols "On"
        links { "yaml-cpp" } -- append yaml-cpp library for Windows
