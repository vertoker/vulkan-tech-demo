
#--------------------------------------------------------------------------------
# Init
#--------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20)
project(vulkan-tech-demo VERSION 1.0 LANGUAGES CXX) # Your project name here
set(CMAKE_CXX_STANDARD 20)

# Other features
set(DISABLE_CONSOLE 0)

#--------------------------------------------------------------------------------
# Src & Include
#--------------------------------------------------------------------------------

# Set variables for short
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Recurse collect every file in include/ and src/ folders
file(GLOB_RECURSE INCLUDES CONFIGURE_DEPENDS "${INCLUDE_DIR}/*.h" "${INCLUDE_DIR}/*.hpp")
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.c" "${SOURCE_DIR}/*.cpp")

# add include/ folder as root for all header files
include_directories("${INCLUDE_DIR}")

# finally add all founded include/ and src/ files into compiler as a program
add_executable("${CMAKE_PROJECT_NAME}" "${INCLUDES}" "${SOURCES}")

#file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS "src/*.h" "src/*.c" "src/*.hpp" "src/*.cpp")
#add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

#--------------------------------------------------------------------------------
# Resources (feature)
#--------------------------------------------------------------------------------

# Use it in development
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
# Use it in build
#target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

#--------------------------------------------------------------------------------
# Libraries
#--------------------------------------------------------------------------------

# Include folder in directory (etc. combine folders)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/") # lib

# Add Local libraries
add_subdirectory("lib/entt")

# Add Package libraries (vcpkg)
find_package(fmt CONFIG REQUIRED)
find_package(boost_dll CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Link Local Libraries
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE EnTT::EnTT)

# Link Package Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Boost::dll)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE tinyobjloader::tinyobjloader)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Vulkan::Vulkan)

#--------------------------------------------------------------------------------
# Utility
#--------------------------------------------------------------------------------

# Disable console
if (DISABLE_CONSOLE)
	if(MSVC) # If using the VS compiler...
		target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
		set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
	endif()
	# For others idk how to solve this, search through the internet or something
endif()

# Scripts execution

if (WIN32)#TODO
	execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/compile-shaders.bat")
	execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy-resources.bat")
endif (WIN32)

if (UNIX)
	execute_process(COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/scripts/compile-shaders.sh")
	execute_process(COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy-resources.sh")
endif (UNIX)
