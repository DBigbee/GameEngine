include "Dependencies.lua"

workspace "GameEngine"
	architecture "x86_64"
	startproject "GameEngine"
	
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
	group ""
	
	include "GameEngine"
