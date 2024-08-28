@echo off
rem Compile shaders before start

set PROJECT_PATH=%cd%\
set RESOURCES=%cd%\resources\

rem Build folders
set DEBUG_BUILD=%cd%\out\build\x64-Debug\resources\
set RELEASE_BUILD=%cd%\out\build\x64-Release\resources\

@echo on

md %RESOURCES%
md %DEBUG_BUILD%
md %RELEASE_BUILD%

@RD /S /Q %DEBUG_BUILD%
@RD /S /Q %RELEASE_BUILD%
xcopy /s %RESOURCES% %DEBUG_BUILD%
xcopy /s %RESOURCES% %RELEASE_BUILD%

pause
