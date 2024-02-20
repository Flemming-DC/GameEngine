Engine = project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"

    files { "**.h", "**.cpp", "**.hpp", "**.inl" } 
   
    includedirs {
        "../Engine/src",
        "../Engine/src/Diverse",
        "../Engine/src/Tools",
        "../Engine/src/ECS",
        "../Engine/src/Rendering",
        "../Engine/src/Rendering/Surface",
        "../Engine/src/Rendering/BufferHandling",
        "../Engine/src/Physics2D",
        "../Engine/src/Physics2D/Colliders",
        "../Engine/src/Physics2D/Calculation",
        "../Engine/src/Saveables",
        "../Engine/Dependencies/SrcDependencies/stduuid",
        "../Engine/Dependencies/SrcDependencies",
        "../Engine/Dependencies/BinDependencies/OpenGL/include",
        "../Engine/Dependencies/BinDependencies/yaml-cpp-master/include",
        "../Engine/Dependencies/BinDependencies/GLEW/include"
    }

    links {
        -- "/NODEFAULTLIB:LIBCMT;" -- insert manually without .lib extension
        "glew32s",
        "glfw3", 
        "Opengl32",
        "Glu32", 
        "yaml-cppd" -- 'd' means debug. You also have a release version.
    }
    ignoredefaultlibraries { "/NODEFAULTLIB:LIBCMT" }

    libdirs { -- evt. copy all libs to one folder and grab them from there
        "../Engine/Dependencies/BinDependencies/OpenGL/lib-vc2022",
        "../Engine/Dependencies/BinDependencies/yaml-cpp-master/build/Debug",
        "../Engine/Dependencies/BinDependencies/GLEW/lib/Release/Win32"
    }


    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { }

    filter "configurations:Debug"
        defines { "GLEW_STATIC", "WIN32", "_DEBUG", "_CONSOLE" } 
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "GLEW_STATIC", "WIN32", "NDEBUG", "_CONSOLE" } 
        runtime "Release"
        --optimize "On" -- this breaks openGL
        symbols "On"
