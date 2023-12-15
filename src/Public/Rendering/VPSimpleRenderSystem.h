#pragma once


#pragma once
#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"
#include <vector>
#include <memory>


#include "VPPipeline.h"
#include "VPEngineDevice.h"
#include "VPGameObject.h"
#include "VPCamera.h"

namespace VP {
    class VPSimpleRenderSystem {
    public:
        VPSimpleRenderSystem(VPEngineDevice& device, VkRenderPass renderPass);
        ~VPSimpleRenderSystem();

        VPSimpleRenderSystem(const VPSimpleRenderSystem&) = delete;
        VPSimpleRenderSystem &operator=(const VPSimpleRenderSystem&) = delete;


        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VPGameObject>& gameObjects, const VPCamera& camera);

    private:

    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

    private:
        void createPipeline(VkRenderPass renderPass);
        void createPipelineLayout();



        VPEngineDevice& Device;

        std::unique_ptr<VPPipeline> Pipeline;
        VkPipelineLayout pipelineLayout;

    };

}

