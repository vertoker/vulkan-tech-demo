@echo off
rem Compile shaders before start

cd ..

set PROJECT_PATH=%cd%\
set RESOURCES_INPUT=%cd%\src\shaders\
set RESOURCES_OUTPUT=%cd%\resources\ShaderCache\

@echo on

md %RESOURCES_OUTPUT%

glslc %RESOURCES_INPUT%simple.vert -o %RESOURCES_OUTPUT%simple.vert.spv
glslc %RESOURCES_INPUT%simple.frag -o %RESOURCES_OUTPUT%simple.frag.spv

pause
