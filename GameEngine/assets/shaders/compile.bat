@echo off

setlocal enabledelayedexpansion
for /r %%i in (*.vert, *.frag) do (
	echo %%i
	C:/VulkanSDK/1.3.239.0/Bin/glslc.exe "%%~i" -o "%%~i.spv"
)
PAUSE