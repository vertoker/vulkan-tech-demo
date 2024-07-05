# Core CMake

cmake_minimum_required(VERSION 3.12)
project(executable) # Your project name here
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Sources

# Use it if you're normal
#add_executable(executable "src/main.cpp")

# Use it if you're crazy
file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")
add_executable(executable ${SOURCES})

# Resources

# Use it in development
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
# Use it in build
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

# Libraries

# Include folder in directory (etc. combine folders)
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/")

# Add your libraries here
#add_subdirectory(lib/glm)
#add_subdirectory(lib/glfw)

# Add your libraries in a row through
#target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE 
#	glm, glfw)
