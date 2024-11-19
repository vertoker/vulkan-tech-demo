@echo off
rem Compile shaders before start

cd ../..

set PROJECT_PATH=%cd%\
set RESOURCES=%cd%\resources\

rem Build folders
set DEBUG_BUILD=%cd%\out\Debug\resources\
set RELEASE_BUILD=%cd%\out\Release\resources\

@echo on

md %RESOURCES%
md %DEBUG_BUILD%
md %RELEASE_BUILD%

@RD /S /Q %DEBUG_BUILD%
@RD /S /Q %RELEASE_BUILD%
xcopy /s %RESOURCES% %DEBUG_BUILD%
xcopy /s %RESOURCES% %RELEASE_BUILD%
