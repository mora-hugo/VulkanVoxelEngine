
#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "App/Window/VPWindow.h"
#include "Graphics/Pipeline/VPPipeline.h"
#include "App/Utils/VPEngineDevice.h"
#include "Graphics/Pipeline/VPEngineSwapChain.h"
namespace VP {
    class App {
    public:
        App();
        ~App();

        App(const App&) = delete;
        App &operator=(const App&) = delete;



    private:
        void run();

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    private:
        void sierpiski(std::vector<VPModel::Vertex> &vertices,
                       int depth,
                       glm::vec2 left,
                       glm::vec2 right,
                       glm::vec2 top);
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();
        VPWindow Window{WIDTH, HEIGHT, "Vulkan project window"};
        VPEngineDevice Device{Window};
        VPEngineSwapChain SwapChain {Device,Window.getExtent()};
        std::unique_ptr<VPPipeline> Pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<VPModel> model;

    };

}

