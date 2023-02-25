@echo off
pushd %~dp0\..\
call GameEngine\vendor\Premake\bin\premake5.exe vs2022
popd
PAUSE