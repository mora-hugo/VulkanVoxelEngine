
#pragma once

#include "VPEngineDevice.h"

#include <memory>

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
            glm::vec3 normal;
            glm::vec2 texCoord;

            bool operator==(const Vertex& other) const {
                return position == other.position && color == other.color && normal == other.normal && texCoord == other.texCoord;
            }

        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string& filepath);

            static std::shared_ptr<VPModel> createModelFromFile(VPEngineDevice& device, const std::string& filepath);

        };
        VPModel(VPEngineDevice& device, const VP::VPModel::Builder& builder);
        ~VPModel();

        VPModel(const VPModel&) = delete;
        VPModel &operator=(const VPModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffer(const std::vector<Vertex>& vertices);
            void createIndexBuffer(const std::vector<uint32_t>& indices);

            VPEngineDevice& device;

            /* Vertex buffer */
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;

            /* Index buffer */
            bool hasIndexBuffer = false;
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;
            uint32_t indexCount;
    };

}


