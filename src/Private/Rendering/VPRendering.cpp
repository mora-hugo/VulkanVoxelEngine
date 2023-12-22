
#include "Rendering/App.h"
#define GLFW_INCLUDE_VULKAN



#include "GLFW/glfw3.h"
#include "glm/gtc/constants.hpp"
#include <stdexcept>
#include <array>

#include "Rendering/VPRendering.h"


struct SimplePushConstantData {
    glm::mat2 transform {1.0f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};


VP::VPRenderer::VPRenderer(VPWindow& window, VPEngineDevice& device) : Window(window), Device(device) {

    recreateSwapChain();
    createCommandBuffers();

}

VP::VPRenderer::~VPRenderer() {
    freeCommandBuffers();

}



void VP::VPRenderer::createCommandBuffers() {
    commandBuffers.resize(VPEngineSwapChain::MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(Device.device(),&allocInfo,commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }





}

void VP::VPRenderer::recreateSwapChain() {
    auto extent = Window.getExtent();
    while(extent.width == 0 || extent.height == 0) {
        extent = Window.getExtent();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(Device.device());

    if(SwapChain == nullptr) {
        SwapChain = std::make_unique<VPEngineSwapChain>(Device,extent);
    } else {
        std::shared_ptr<VPEngineSwapChain> oldSwapChain = std::move(SwapChain);
        SwapChain = std::make_unique<VPEngineSwapChain>(Device, extent, oldSwapChain);

        if(!oldSwapChain->compareSwapFormats(*SwapChain.get())){
            throw std::runtime_error("Swap chain image(or depth) format has changed");
        }



    }


    //CREATE PIPELINE BEFORE HERE
}



void VP::VPRenderer::freeCommandBuffers() {
    vkFreeCommandBuffers(Device.device(),Device.getCommandPool(),static_cast<uint32_t>(commandBuffers.size()),commandBuffers.data());
    commandBuffers.clear();
}

VkCommandBuffer VP::VPRenderer::BeginFrame() {
    assert(!isFrameStarted && "Can't call BeginFrame while already in progress");

    auto result = SwapChain->acquireNextImage(&currentImageIndex);

    if(result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return nullptr;
    }

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    isFrameStarted = true;

    auto commandBuffer = GetCurrentCommandBuffer();

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    //IMGUI
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if(vkBeginCommandBuffer(commandBuffer,&beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer");
    }




    return commandBuffer;



}

void VP::VPRenderer::EndFrame() {
    assert(isFrameStarted && "Can't call BeginFrame while already in progress");
    auto commandBuffer = GetCurrentCommandBuffer();



    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }

    auto result = SwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window.WasWindowResized()) {
        Window.ResetWindowResized();
        recreateSwapChain();
    }



    isFrameStarted = false;
    currentFrameIndex = (currentFrameIndex + 1) % VPEngineSwapChain::MAX_FRAMES_IN_FLIGHT;
}

void VP::VPRenderer::BeginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Can't call BeginFrame while already in progress");
    assert(commandBuffer == GetCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = SwapChain->getRenderPass();
    renderPassInfo.framebuffer = SwapChain->getFrameBuffer(currentImageIndex);

    renderPassInfo.renderArea.offset = {0,0};
    renderPassInfo.renderArea.extent = SwapChain->getSwapChainExtent();

    std::array<VkClearValue,2> clearValues{};
    clearValues[0].color = {0.01f,0.01f,0.01f,1.0f};
    clearValues[1].depthStencil = {1.0f,0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer,&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(SwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(SwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, SwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);



}

void VP::VPRenderer::EndSwapChainRenderPass(VkCommandBuffer commandBuffer) {
    assert(isFrameStarted && "Can't call EndFrame while already in progress");
    assert(commandBuffer == GetCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);

}
