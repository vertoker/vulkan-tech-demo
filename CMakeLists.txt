
#-----
# Init
#-----

cmake_minimum_required(VERSION 3.12)
project(vulkan-tech-demo VERSION 1.0 LANGUAGES CXX) # Your project name here
set(CMAKE_CXX_STANDARD 20)

# Other features
set(DISABLE_CONSOLE 0)

#--------
# Sources
#--------

# Use it if you're normal
#add_executable(vulkan-tech-demo "src/main.cpp")

# Use it if you're crazy
file(GLOB_RECURSE SOURCES CMAKE_CONFIGURE_DEPENDS 
	"src/*.h" "src/*.c" "src/*.hpp" "src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

#--------------------
# Resources (feature)
#--------------------

# Use it in development
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
# Use it in build
#target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

#----------
# Libraries
#----------

# Include folder in directory (etc. combine folders)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib/") # lib

# Add Local libraries
# add_subdirectory("lib/glm")

# Add Package libraries (vcpkg)
find_package(fmt CONFIG REQUIRED)
find_package(boost_dll CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Link Local Libraries
# target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm glfw)

# Link Package Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Boost::dll)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE tinyobjloader::tinyobjloader)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Vulkan::Vulkan)

#--------
# Utility
#--------

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
	execute_process(COMMAND "${CMAKE_PROJECT_NAME}/scripts/compile-shaders.bat")
	execute_process(COMMAND "${CMAKE_PROJECT_NAME}/scripts/copy-resources.bat")
endif (WIN32)

if (UNIX)
	execute_process(COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/scripts/compile-shaders.sh")
	execute_process(COMMAND sh "${CMAKE_CURRENT_SOURCE_DIR}/scripts/copy-resources.sh")
endif (UNIX)
