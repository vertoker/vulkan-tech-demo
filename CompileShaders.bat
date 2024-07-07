@echo off
rem Compile shaders before start

set PROJECT_PATH=%cd%\
rem Paste path to your glslc.exe file here (in Vulkan SDK)
set GLSLC_PATH=D:\SDK\Vulkan\1.3.283.0\Bin\glslc.exe

@echo on
%GLSLC_PATH% %PROJECT_PATH%src/shaders/simple.vert -o %PROJECT_PATH%resources/ShaderCache/simple.vert.spv
%GLSLC_PATH% %PROJECT_PATH%src/shaders/simple.frag -o %PROJECT_PATH%resources/ShaderCache/simple.frag.spv
pause
