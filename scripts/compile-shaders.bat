@echo off
rem Compile shaders before start

setlocal
cd /d %~dp0
cd ..

set RESOURCES_INPUT=%cd%\src\shaders\
set RESOURCES_OUTPUT=%cd%\resources\ShaderCache\

@echo on

md %RESOURCES_OUTPUT%

glslc %RESOURCES_INPUT%world.vert -o %RESOURCES_OUTPUT%world.vert.spv
glslc %RESOURCES_INPUT%world.frag -o %RESOURCES_OUTPUT%world.frag.spv
glslc %RESOURCES_INPUT%point_light.vert -o %RESOURCES_OUTPUT%point_light.vert.spv
glslc %RESOURCES_INPUT%point_light.frag -o %RESOURCES_OUTPUT%point_light.frag.spv
