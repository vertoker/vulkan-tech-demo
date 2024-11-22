
// app
#include "app.hpp"

// libraries
#include <fmt/core.h>
#include <boost/dll.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VulkanAppSettings createAppSettings() {
    VulkanAppSettings appSettings{};

    std::string path = boost::dll::program_location().parent_path().generic_path().string(); // wtf
    std::string shaders = path + "/resources/ShaderCache/";
    std::string models = path + "/resources/models/";

    appSettings.world_vertShaderPath = shaders + std::string("world.vert.spv");
    appSettings.world_fragShaderPath = shaders + std::string("world.frag.spv");
    appSettings.pointLight_vertShaderPath = shaders + std::string("point_light.vert.spv");
    appSettings.pointLight_fragShaderPath = shaders + std::string("point_light.frag.spv");
    appSettings.modelPath = models + std::string("keyboard.obj"); // keyboard smooth-vase flat-vase

    return appSettings;
}

int main() {
#if _DEBUG
    fmt::print("Mode = Debug\n");
#else
    fmt::print("Mode = Release\n");
#endif

    auto appSettings = createAppSettings();

    VulkanApp app{ appSettings };

    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    fmt::print("Stop app");
    return EXIT_SUCCESS;
}