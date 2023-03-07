VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/GameEngine/vendor/GLFW/include"
IncludeDir["GLM"] = "%{wks.location}/GameEngine/vendor/GLM/include"
IncludeDir["STBImage"] = "%{wks.location}/GameEngine/vendor/STBImage"
IncludeDir["Vulkan"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}
LibraryDir["Vulkan"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.Vulkan}/vulkan-1.lib"