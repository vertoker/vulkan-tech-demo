# CMake Template

(**If you clone it, you better delete this README.md file**)

My personal CMake template for all of my projects in C++

Build special for Visual Studio environment, doesn't test on other IDE

## How to use

1. Clone it. If you don't know how, well, you need to learn basics

2. Open in Visual Studio (inside folder "Open with Visual Studio")

3. In CMakeLists.txt follow coments, setting to your project. You must check:
    - CMake minimum version (recommend not below 3.12)
    - Project name (and in `add_executable` functions)
    - C++ language standard

4. Update remote `origin` to your GitHub (or another remote git service) package.
I usually just write this commands
    - `git remote remove origin`
    - `git remote add origin <paste your link here>`


Big thanks to @meemknight, this template based on his guide 
(and many other texts through entire internet)
