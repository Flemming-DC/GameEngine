include "../Game/BuildGame.lua"
project "Game-InEditor"
    kind "ConsoleApp"
    language(Game.language)
    cppdialect(Game.cppdialect)
    targetdir(Game.targetdir)
    staticruntime(Game.staticruntime)
    
    -- dont use engine.files, since it creates a mess in Visual Studio
    files { "Source/**.h", "Source/**.cpp", "Source/**.hpp", "Source/**.inl" } 
   
    includedirs {
        "../Engine/Source/", 
        "../Editor/Source/", 
        Game.includedirs
    }
    links {
		"Editor",
        Game.links
    }
    ignoredefaultlibraries(Game.ignoredefaultlibraries)
    libdirs(Game.libdirs)

    targetdir(Game.targetdir)
    objdir(Game.objdir)

    filter "system:windows"
        systemversion "latest"
        defines { }
        links { "dbghelp" } -- append dbghelp library for Windows

    filter "configurations:Debug"
        defines { "GLEW_STATIC", "WIN32", "_DEBUG", "_CONSOLE", "InEditor" } 
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "GLEW_STATIC", "WIN32", "NDEBUG", "_CONSOLE", "InEditor" } 
        runtime "Release"
        optimize "On" 
        symbols "On"
