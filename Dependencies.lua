VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/GameEngine/vendor/GLFW/include"
IncludeDir["GLM"] = "%{wks.location}/GameEngine/vendor/glm"
IncludeDir["ASSIMP"] = "%{wks.location}/GameEngine/vendor/Assimp/include"
IncludeDir["STBImage"] = "%{wks.location}/GameEngine/vendor/stb_image"
IncludeDir["Entt"] = "%{wks.location}/GameEngine/vendor/ENTT"
IncludeDir["Vulkan"] = "%{VULKAN_SDK}/Include"
IncludeDir["SPDLOG"] = "%{wks.location}/GameEngine/vendor/spdlog/include"

LibraryDir = {}
LibraryDir["Vulkan"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.Vulkan}/vulkan-1.lib"
Library["ShaderC_Debug"] = "%{LibraryDir.Vulkan}/shaderc_shared.lib"
Library["ShaderC_Release"] = "%{LibraryDir.Vulkan}/shaderc.lib"

-- Windows
--Library["WinSock"] = "Ws2_32.lib"
--Library["WinMM"] = "Winmm.lib"
--Library["WinVersion"] = "Version.lib"
--Library["BCrypt"] = "Bcrypt.lib"