
// app
#include "app.hpp"

// libraries
#include <fmt/core.h>
#include <boost/dll.hpp>

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VulkanAppSettings createAppSettings() {
    VulkanAppSettings appSettings{};

    auto path = boost::dll::program_location().parent_path().generic_path().string();
    auto shaders = path + "/resources/ShaderCache/";
    auto models = path + "/resources/models/";
    appSettings.vertShaderPath = shaders + std::string("world.vert.spv");
    appSettings.fragShaderPath = shaders + std::string("world.frag.spv");
    appSettings.modelPath = models + std::string("keyboard.obj");
    //keyboard smooth-vase flat-vase

    return appSettings;
}

int main() {
#if _DEBUG
    fmt::print("Mode = Debug\n");
#else
    fmt::print("Mode = Release\n");
#endif

    auto appSettings = createAppSettings();

    std::cout << appSettings.vertShaderPath << std::endl;
    std::cout << appSettings.fragShaderPath << std::endl;

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