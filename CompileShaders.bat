@echo off
rem Compile shaders before start

set PROJECT_PATH=%cd%\
rem Paste path to your glslc.exe file here (in Vulkan SDK)
set GLSLC_PATH=D:\SDK\Vulkan\1.3.283.0\Bin\glslc.exe

set RESOURCES_INPUT=%cd%\src\shaders\
set RESOURCES_OUTPUT=%cd%\resources\ShaderCache\

@echo on

md %RESOURCES_OUTPUT%

%GLSLC_PATH% %RESOURCES_INPUT%simple.vert -o %RESOURCES_OUTPUT%simple.vert.spv
%GLSLC_PATH% %RESOURCES_INPUT%simple.frag -o %RESOURCES_OUTPUT%simple.frag.spv

pause
