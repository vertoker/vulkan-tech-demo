
#include <systems/imgui_system.hpp>
#include <render/swapchain.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

// Based on https://github.com/ocornut/imgui/blob/master/examples/example_glfw_vulkan/main.cpp

static void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if (err < 0)
        abort();
}

ImguiSystem::ImguiSystem(VulkanDevice& device, VkRenderPass renderPass, VkDescriptorPool descriptorPool, GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // enable gamepad

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = device.instance();
    init_info.PhysicalDevice = device.physicalDevice();
    init_info.Device = device.device();
    init_info.QueueFamily = device.findPhysicalQueueFamilies().graphicsFamily;
    init_info.Queue = device.graphicsQueue();
    init_info.DescriptorPool = descriptorPool;
    init_info.RenderPass = renderPass;

    init_info.Subpass = 0;
    init_info.MinImageCount = VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.ImageCount = VulkanSwapChain::MAX_FRAMES_IN_FLIGHT;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = check_vk_result;

    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_Init(&init_info);
}
ImguiSystem::~ImguiSystem()
{
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiSystem::execute(VulkanFrameInfo& frameInfo)
{
    // @TODO finish imgui
    // use https://github.com/ocornut/imgui/blob/master/examples/example_glfw_vulkan/main.cpp
    
    /*bool show_demo_window = true;

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();*/
}
