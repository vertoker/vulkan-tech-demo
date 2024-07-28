
#include "gravity_app.h"
#include "utility/log.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VulkanPhysicsAppSettings createAppSettings() {
    VulkanPhysicsAppSettings appSettings{};

    appSettings.vertShaderPath = RESOURCES_PATH + std::string("ShaderCache/simple.vert.spv");
    appSettings.fragShaderPath = RESOURCES_PATH + std::string("ShaderCache/simple.frag.spv");

    return appSettings;
}

int main() {
    Log("Start app");

    auto appSettings = createAppSettings();

    std::cout << appSettings.vertShaderPath << std::endl;
    std::cout << appSettings.fragShaderPath << std::endl;

    VulkanPhysicsApp app{ appSettings };

    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    Log("Stop app");
    return EXIT_SUCCESS;
}