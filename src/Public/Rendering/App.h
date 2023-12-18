
#pragma once
#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"
#include <vector>
#include <memory>
#include "VPWindow.h"
#include "VPEngineDevice.h"
#include "VPEngineSwapChain.h"
#include "VPGameObject.h"
#include "VPRendering.h"
#include "glm/gtc/constants.hpp"
#include "VPCamera.h"
#include "Inputs/VPInputManager.h"

namespace VP {




    class App {
    public:
        App();
        ~App();

        App(const App&) = delete;
        App &operator=(const App&) = delete;

        VPInputManager& GetInputManager() { return inputManager; }




    private:
        void run();

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    private:
        void loadGameObjects();
        std::unique_ptr<VPModel> createCubeModel(VPEngineDevice& device, glm::vec3 offset);
        VPWindow Window{WIDTH, HEIGHT, "Vulkan project window"};
        VPEngineDevice Device{Window};
        VPRenderer Renderer{Window, Device};
        VPCamera camera{};
        VPInputManager inputManager{};



        std::vector<VPGameObject> gameObjects;

    };

}

