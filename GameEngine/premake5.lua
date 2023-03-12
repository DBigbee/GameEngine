project "GameEngine"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "src/pch.cpp"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_VULKUN"
	}
	
	includedirs
	{
		"src",
		"%{IncludeDir.STBImage}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.Entt}"
	}
	
	links
	{
		"GLFW",
		"Assimp",
		"%{Library.Vulkan}"
	}
	
	filter "files:src/main.cpp"
	flags { "NoPCH" }

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