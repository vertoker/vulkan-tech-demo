
#include "gravity_app.h"
#include "utility/log.h"

#include "boost/dll.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VulkanPhysicsAppSettings createAppSettings() {
    VulkanPhysicsAppSettings appSettings{};

    auto path = boost::dll::program_location().parent_path().generic_path().string();
    path = path + "/resources/ShaderCache/";
    appSettings.vertShaderPath = path + std::string("simple.vert.spv");
    appSettings.fragShaderPath = path + std::string("simple.frag.spv");

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