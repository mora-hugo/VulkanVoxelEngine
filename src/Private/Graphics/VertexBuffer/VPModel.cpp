//
// Created by hugom on 04/11/2023.
//

#include "Graphics/VertexBuffer/VPModel.h"

VP::VPModel::VPModel(VP::VPEngineDevice &device, const std::vector<Vertex> &vertices) : device{device} {
    createVertexBuffer(vertices);
}

VP::VPModel::~VPModel() {
    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void VP::VPModel::draw(VkCommandBuffer commandBuffer) {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);


}

void VP::VPModel::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] {vertexBuffer};
    VkDeviceSize offsets[] {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

}

void VP::VPModel::createVertexBuffer(const std::vector<Vertex> &vertices) {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
    device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

    void* data;
    vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(device.device(), vertexBufferMemory);
}


std::vector<VkVertexInputBindingDescription> VP::VPModel::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> VP::VPModel::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].location = 0; //position in the vertex shader
    bindingDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    bindingDescriptions[0].offset = offsetof(Vertex, position);
    return bindingDescriptions;
}
