@echo off
rem Compile shaders before start

set PROJECT_PATH=%cd%\
rem Paste path to your glslc.exe file here (in Vulkan SDK)
set GLSLC_PATH=D:\SDK\Vulkan\1.3.283.0\Bin\glslc.exe

set RESOURCES_INPUT=%cd%\src\shaders\
set RESOURCES_OUTPUT=%cd%\resources\ShaderCache\

rem Build folders
set DEBUG_BUILD=%cd%\out\build\x64-Debug\resources\ShaderCache\
set RELEASE_BUILD=%cd%\out\build\x64-Release\resources\ShaderCache\

@echo on

md %RESOURCES_OUTPUT%
md %DEBUG_BUILD%
md %RELEASE_BUILD%

%GLSLC_PATH% %RESOURCES_INPUT%simple.vert -o %RESOURCES_OUTPUT%simple.vert.spv
copy %RESOURCES_OUTPUT%simple.vert.spv %DEBUG_BUILD%simple.vert.spv
copy %RESOURCES_OUTPUT%simple.vert.spv %RELEASE_BUILD%simple.vert.spv

%GLSLC_PATH% %RESOURCES_INPUT%simple.frag -o %RESOURCES_OUTPUT%simple.frag.spv
copy %RESOURCES_OUTPUT%simple.frag.spv %DEBUG_BUILD%simple.frag.spv
copy %RESOURCES_OUTPUT%simple.frag.spv %RELEASE_BUILD%simple.frag.spv

pause
