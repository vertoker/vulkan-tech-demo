
#include "app.h"
#include "utility/log.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

VulkanAppSettings createAppSettings() {
    VulkanAppSettings appSettings{};

    PipelineConfigInfo pipelineInfo{};

    appSettings.vertShaderPath = RESOURCES_PATH + std::string("ShaderCache/simple.vert.spv");
    appSettings.fragShaderPath = RESOURCES_PATH + std::string("ShaderCache/simple.frag.spv");

    appSettings.pipelineInfo = VulkanPipeline::defaultConfigInfo(appSettings.screenWidth, appSettings.screenHeight);
    return appSettings;
}

int main() {
    Log("Start app");

    auto appSettings = createAppSettings();
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