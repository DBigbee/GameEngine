VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/GameEngine/vendor/GLFW/include"
IncludeDir["GLM"] = "%{wks.location}/GameEngine/vendor/GLM/include"
IncludeDir["ASSIMP"] = "%{wks.location}/GameEngine/vendor/Assimp/include"
IncludeDir["STBImage"] = "%{wks.location}/GameEngine/vendor/STBImage"
IncludeDir["Entt"] = "%{wks.location}/GameEngine/vendor/ENTT"
IncludeDir["Vulkan"] = "%{VULKAN_SDK}/Include"


LibraryDir = {}
LibraryDir["Vulkan"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.Vulkan}/vulkan-1.lib"
Library["ShaderC_Debug"] = "%{LibraryDir.Vulkan}/shaderc_shared.lib"
Library["ShaderC_Release"] = "%{LibraryDir.Vulkan}/shaderc.lib"