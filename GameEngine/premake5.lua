project "GameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"
	
	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "src/pch.cpp"
	
	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	
	includedirs
	{
		"src",
		"src/GameEngine",
		"src/Platform",
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
		"GLFW",
		"Assimp",
		"%{Library.Vulkan}"
	}
	
	filter "files:src/main.cpp"
	flags { "NoPCH" }

	filter "system:windows"
      systemversion "latest"
	  
	--links
	--{
	--	"%{Library.WinSock}",
	--	"%{Library.WinMM}",
	--	"%{Library.WinVersion}",
	--	"%{Library.BCrypt}",
	--}
	--prebuildcommands{"%{wks.location}%{prj.name}\\assets\\shaders\\compile.bat"}

	filter "configurations:Debug"
		defines "GE_DEBUG"
		runtime "Debug"
		symbols "on"

		links
		{
			"%{Library.ShaderC_Debug}",
		}
		
	filter "configurations:Release"
		defines "GE_RELEASE"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
		}
		
	filter "configurations:Dist"
		defines "GE_DIST"
		runtime "Release"
		optimize "on"

		links
		{
			"%{Library.ShaderC_Release}",
		}

