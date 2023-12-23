//
// Created by hugom on 04/11/2023.
//

#include "Rendering/VPModel.h"
#include "Rendering/VPUtils.h"

//libs
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <iostream>

namespace std {
    template<> struct hash<VP::VPModel::Vertex> {
        size_t operator()(VP::VPModel::Vertex const& vertex) const {
            size_t seed = 0;
            VP::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
            return seed;
        }
    };
}
VP::VPModel::VPModel(VP::VPEngineDevice &device, const VP::VPModel::Builder& builder) : device{device} {
    createVertexBuffer(builder.vertices);
    createIndexBuffer(builder.indices);
}

VP::VPModel::~VPModel() {
    /* Vertex buffer */
    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);

    /* Index buffer */
    if(hasIndexBuffer) {
        vkDestroyBuffer(device.device(), indexBuffer, nullptr);
        vkFreeMemory(device.device(), indexBufferMemory, nullptr);
    }


}

void VP::VPModel::draw(VkCommandBuffer commandBuffer) {
    if(hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    }
    else {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }

}

void VP::VPModel::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] {vertexBuffer};
    VkDeviceSize offsets[] {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

    if(hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }



}

void VP::VPModel::createVertexBuffer(const std::vector<Vertex> &vertices) {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


    /* Link data to the GPU */
    void* data;
    vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device.device(), stagingBufferMemory);


    // Create vertex buffer
    device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void VP::VPModel::createIndexBuffer(const std::vector<uint32_t> &indices) {
    indexCount = static_cast<uint32_t>(indices.size());
    hasIndexBuffer = indexCount > 0;

    if(!hasIndexBuffer) return;

    VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    device.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


    /* Link data to the GPU */
    void* data;
    vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(device.device(), stagingBufferMemory);


    // Create vertex buffer
    device.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}


std::vector<VkVertexInputBindingDescription> VP::VPModel::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> VP::VPModel::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0; //position in the vertex shader
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1; //position in the vertex shader
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    return attributeDescriptions;
}

void VP::VPModel::Builder::loadModel(const std::string &filepath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials{};
    std::string warn, err;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    vertices.clear();
    indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for(const auto &shape : shapes) {
        for(const auto &index : shape.mesh.indices) {

            Vertex vertex{};

            if(index.vertex_index >= 0) {
                vertex.position = {
                    attrib.vertices[3*index.vertex_index + 0],
                    attrib.vertices[3*index.vertex_index + 1],
                    attrib.vertices[3*index.vertex_index + 2]
                };

                //Color support
                auto colorIndex = 3*index.vertex_index + 2;
                if(colorIndex < attrib.colors.size()) {
                    vertex.color = {
                        attrib.colors[colorIndex - 2],
                        attrib.colors[colorIndex - 1],
                        attrib.colors[colorIndex - 0]
                    };
                }
                else {
                    //default color
                    vertex.color = {1.0f, 1.0f, 1.0f};
                }
            }

            if(index.normal_index >= 0) {
                vertex.normal = {
                        attrib.normals[3*index.vertex_index + 0],
                        attrib.normals[3*index.vertex_index + 1],
                        attrib.normals[3*index.vertex_index + 2]
                };
            }

            if(index.texcoord_index >= 0) {
                vertex.texCoord = {
                        attrib.texcoords[3*index.vertex_index + 0],
                        attrib.texcoords[3*index.vertex_index + 1],
                };
            }

            if(uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }

}

std::shared_ptr<VP::VPModel>
VP::VPModel::Builder::createModelFromFile(VP::VPEngineDevice &device, const std::string &filepath) {
    Builder builder{};
    builder.loadModel(filepath);
    std::cout << "Vertex count: " << builder.vertices.size() << std::endl;
    return std::make_shared<VPModel>(device, builder);
}

