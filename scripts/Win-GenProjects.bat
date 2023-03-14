@echo off

pushd %~dp0\..\
call GameEngine\vendor\Premake\premake5.exe vs2022
popd
PAUSE