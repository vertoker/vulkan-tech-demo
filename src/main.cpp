
#include "app.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

constexpr auto HIDE_CONSOLE = 1;

int main() {
    VulkanAppSettings appSettings {};
    appSettings.VertShaderPath = RESOURCES_PATH + std::string("ShaderCache/simple.vert.spv");
    appSettings.FragShaderPath = RESOURCES_PATH + std::string("ShaderCache/simple.frag.spv");

    VulkanApp app{ appSettings };

    try {
        app.run();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}