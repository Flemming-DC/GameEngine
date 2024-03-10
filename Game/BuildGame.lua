include "../Engine/BuildEngine.lua"
project "Game"
    kind "ConsoleApp"
    language(Engine.language)
    cppdialect(Engine.cppdialect)
    targetdir(Engine.targetdir)
    staticruntime(Engine.staticruntime)
    
    -- dont use engine.files, since it creates a mess in Visual Studio
    files { "src/**.h", "src/**.cpp", "src/**.hpp", "src/**.inl" } 
   
    includedirs {
        "../Game/src",
        "../Game/src/Components",
        "../Game/src/Scenes",
        Engine.includedirs
    }
    links {
        "Engine",
        Engine.links
    }
    ignoredefaultlibraries(Engine.ignoredefaultlibraries)
    libdirs(Engine.libdirs)

    targetdir(Engine.targetdir)
    objdir(Engine.objdir)

    filter "system:windows"
        systemversion "latest"
        defines { }
        links { "dbghelp" } -- append dbghelp library for Windows

    filter "configurations:Debug"
        defines { "GLEW_STATIC", "WIN32", "_DEBUG", "_CONSOLE" } 
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "GLEW_STATIC", "WIN32", "NDEBUG", "_CONSOLE" } 
        runtime "Release"
        --optimize "On" -- this breaks openGL
        symbols "On"
