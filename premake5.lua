include "Dependencies.lua"

workspace "GameEngine"
	architecture "x86_64"
	startproject "GameProject"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	group "Dependencies"
		include "GameEngine/vendor/GLFW"
		include "GameEngine/vendor/Assimp"
	group ""
	
	group "Core"
		include "GameEngine"
	group ""
	
	include "GameProject"