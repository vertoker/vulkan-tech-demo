@echo off
rem Compile shaders before start

setlocal
cd /d %~dp0
cd ..

set SOURCE=%cd%\resources
set DEBUG=%cd%\out\Debug\resources
set RELEASE=%cd%\out\Release\resources

@echo on

RD /S /Q "%DEBUG%"
RD /S /Q "%RELEASE%"

md "%SOURCE%"
md "%DEBUG%"
md "%RELEASE%"

xcopy /s "%SOURCE%" "%DEBUG%"
xcopy /s "%SOURCE%" "%RELEASE%"
