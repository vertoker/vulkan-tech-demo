@echo off
rem Compile shaders before start

cd ../..

set PROJECT_PATH=%cd%\
set RESOURCES_INPUT=%cd%\src\shaders\
set RESOURCES_OUTPUT=%cd%\resources\ShaderCache\

@echo on

md %RESOURCES_OUTPUT%

glslc %RESOURCES_INPUT%world.vert -o %RESOURCES_OUTPUT%world.vert.spv
glslc %RESOURCES_INPUT%world.frag -o %RESOURCES_OUTPUT%world.frag.spv

pause
