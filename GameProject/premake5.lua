project "GameProject"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	includedirs
	{
		"%{wks.location}/GameEngine/src/GameEngine",
		"%{wks.location}/GameEngine/src",
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.STBImage}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.Entt}"
	}
	
	links
	{
		"GameEngine"
	}

	filter "system:windows"
      systemversion "latest"
	  
	filter "configurations:Debug"
		defines "GE_DEBUG"
		runtime "Debug"
		symbols "on"

		
	filter "configurations:Release"
		defines "GE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "GE_DIST"
		runtime "Release"
		optimize "on"
