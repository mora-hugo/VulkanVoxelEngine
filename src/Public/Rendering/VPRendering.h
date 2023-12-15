#pragma once


#pragma once
#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"
#include <vector>
#include <memory>
#include <cassert>

#include "VPWindow.h"
#include "VPEngineDevice.h"
#include "VPEngineSwapChain.h"

namespace VP {
    class VPRenderer {
    public:
        VPRenderer(VPWindow& window, VPEngineDevice& device);
        ~VPRenderer();

        VPRenderer(const VPRenderer&) = delete;
        VPRenderer &operator=(const VPRenderer&) = delete;
        float GetAspectRatio() const { return SwapChain->extentAspectRatio(); }
        bool IsFrameInProgress() const { return isFrameStarted; }

        VkRenderPass GetSwapChainRenderPass() const { return SwapChain->getRenderPass(); }
        VkCommandBuffer GetCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }


        int GetFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }
        VkCommandBuffer BeginFrame();
        void EndFrame();

        void BeginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void EndSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:

    public:


    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();


        VPWindow &Window;
        VPEngineDevice &Device;

        std::unique_ptr<VPEngineSwapChain> SwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex = 0;
        int currentFrameIndex = 0;
        bool isFrameStarted = false;

    };

}

