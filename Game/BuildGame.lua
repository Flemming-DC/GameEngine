include "../Engine/BuildEngine.lua"
project "Game"
    kind "ConsoleApp"
    language(Engine.language)
    cppdialect(Engine.cppdialect)
    targetdir(Engine.targetdir)
    staticruntime(Engine.staticruntime)
    
    -- dont use engine.files, since it creates a mess in Visual Studio
    files { "src/**.h", "src/**.cpp", "src/**.hpp", "src/**.inl" } 
   
    includedirs(Engine.includedirs)
    links {
        "Engine",
        "glew32s",
        "glfw3", 
        "Opengl32",
        "Glu32", 
        "yaml-cppd"
    }
    ignoredefaultlibraries(Engine.ignoredefaultlibraries)
    libdirs(Engine.libdirs)

    targetdir(Engine.targetdir)
    objdir(Engine.objdir)

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
