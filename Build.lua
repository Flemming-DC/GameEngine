-- premake5.lua
toolset "v142"
workspace "GameEngine"
    architecture "x86"
    configurations { "Debug", "Release" }
    startproject "Game"
   
    -- Workspace-wide build options for MSVC
    filter "system:windows"
        buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

	filter { "system:windows", "action:vs*"}
		flags { "MultiProcessorCompile" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group ""
	include "Engine/BuildEngine.lua"
	include "Editor/BuildEditor.lua"
group ""

include "Game/BuildGame.lua"
include "Game/BuildGame-InEditor.lua"

