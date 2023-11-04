
#include "App/App.h"
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <stdexcept>
#include <array>

void VP::App::run() {
    while(!Window.ShouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    // wait all ressources to be free
    vkDeviceWaitIdle(Device.device());
}

VP::App::App() {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
    run();
}

VP::App::~App() {
    vkDestroyPipelineLayout(Device.device(),pipelineLayout,nullptr);

}

void VP::App::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    if (vkCreatePipelineLayout(Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

}

void VP::App::createPipeline() {
    auto PipelineConfig = VPPipeline::GetDefaultPipelineConfigInfo(SwapChain.width(),SwapChain.height());
    PipelineConfig.renderPass = SwapChain.getRenderPass();
    PipelineConfig.pipelineLayout = pipelineLayout;
    Pipeline = std::make_unique<VPPipeline>(Device,"../shaders/simple_shader.vert.spv","../shaders/simple_shader.frag.spv", PipelineConfig);
}

void VP::App::createCommandBuffers() {
    commandBuffers.resize(SwapChain.imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Device.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(Device.device(),&allocInfo,commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers");
    }

    for(int i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffers[i],&beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = SwapChain.getRenderPass();
        renderPassInfo.framebuffer = SwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = SwapChain.getSwapChainExtent();

        std::array<VkClearValue,2> clearValues{};
        clearValues[0].color = {0.01f,0.01f,0.01f,1.0f};
        clearValues[1].depthStencil = {1.0f,0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i],&renderPassInfo,VK_SUBPASS_CONTENTS_INLINE);

        Pipeline->bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i],3,1,0,0);

        vkCmdEndRenderPass(commandBuffers[i]);
        if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer");
        }

    }


}

void VP::App::drawFrame() {
    uint32_t imageIndex;
    auto result = SwapChain.acquireNextImage(&imageIndex);

    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image");
    }

    result = SwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if(result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image");
    }


}
