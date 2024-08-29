
#include "app.h"
#include "utility/log.h"

#include "boost/dll.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VulkanAppSettings createAppSettings() {
    VulkanAppSettings appSettings{};

    auto path = boost::dll::program_location().parent_path().generic_path().string();
    auto shaders = path + "/resources/ShaderCache/";
    auto models = path + "/resources/models/";
    appSettings.vertShaderPath = shaders + std::string("simple.vert.spv");
    appSettings.fragShaderPath = shaders + std::string("simple.frag.spv");
    appSettings.modelPath = models + std::string("flat-vase.obj");//keyboard

    return appSettings;
}

int main() {
#if _DEBUG
    Log("Mode = Debug");
#else
    Log("Mode = Release");
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

    Log("Stop app");
    return EXIT_SUCCESS;
}