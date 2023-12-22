
#include "Rendering/VPSimpleRenderSystem.h."
#define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

#include "GLFW/glfw3.h"
#include "glm/gtc/constants.hpp"
#include <stdexcept>
#include <array>
#include <iostream>

#include "Rendering/VPGameObject.h"
#include "Rendering/VPSimpleRenderSystem.h"


struct SimplePushConstantData {
    glm::mat4 transform {1.0f};
    alignas(16) glm::vec3 color;
};


VP::VPSimpleRenderSystem::VPSimpleRenderSystem(VPEngineDevice& device, VkRenderPass renderPass) : Device(device) {
    createPipelineLayout();
    createPipeline(renderPass);
}

VP::VPSimpleRenderSystem::~VPSimpleRenderSystem() {
    vkDestroyPipelineLayout(Device.device(),pipelineLayout,nullptr);

}

void VP::VPSimpleRenderSystem::createPipelineLayout() {

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);


    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(Device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

}

void VP::VPSimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
    assert(pipelineLayout != VK_NULL_HANDLE && "Cannot create pipeline before pipeline layout is created");


    PipelineConfigInfo pipelineConfig{};
    VPPipeline::GetDefaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    Pipeline = std::make_unique<VPPipeline>(Device,"../shaders/simple_shader.vert.spv","../shaders/simple_shader.frag.spv", pipelineConfig);
}










void VP::VPSimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VPGameObject>& gameObjects, const VPCamera& camera) {
    Pipeline->bind(commandBuffer);
    auto projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();
    for(auto& obj : gameObjects) {

        SimplePushConstantData push{};

        push.color = obj.color;
        push.transform = projectionView * obj.transform.mat4();

        vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}
