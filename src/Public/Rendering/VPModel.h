
#pragma once

#include "VPEngineDevice.h"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

namespace VP {
    class VPModel {
    public:
        struct Vertex {
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            glm::vec3 position;
            glm::vec3 color;

        };
        VPModel(VPEngineDevice& device, const std::vector<Vertex>& vertices);
        ~VPModel();

        VPModel(const VPModel&) = delete;
        VPModel &operator=(const VPModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffer(const std::vector<Vertex>& vertices);

            VPEngineDevice& device;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;

    };

}


