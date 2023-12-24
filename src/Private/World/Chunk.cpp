//
// Created by hugom on 22/12/2023.
//

#include <unordered_map>
#include "World/Chunk.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include "../../lib/fast-noise-lite/FastNoiseLite.h"
#include <iostream>

using namespace VP;


namespace std {
    template<> struct hash<VP::VPModel::Vertex> {
        size_t operator()(VP::VPModel::Vertex const& vertex) const {
            size_t seed = 0;
            VP::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
            return seed;
        }
    };
}
std::shared_ptr<Chunk> Chunk::Build(const glm::vec3& position) {
    std::cout << "Building chunk at " << position.x << " " << position.y << " " << position.z << std::endl;
    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
    chunk->position = position;
    for (size_t x = 0; x < Chunk::DEPTH; ++x) {
        for (size_t y = 0; y < Chunk::HEIGHT; ++y) {
            for (size_t z = 0; z < Chunk::DEPTH; ++z) {
                Block& block = chunk->GetBlock(x, y, z);
                block.parent_chunk = chunk;
                float noiseValue = Chunk::noise.GetNoise((float)x+position.x , (float)z+position.z);
                float height = y + (noiseValue * 100);

                int snappedX = static_cast<int>(x);
                int snappedY = static_cast<int>(height);
                int snappedZ = static_cast<int>(z);
                block.position = glm::vec3{snappedX, snappedY, snappedZ};
                block.id = 1;
            }
        }
    }
    return std::move(chunk);
}

Chunk::Chunk() {
    Chunk::noise.SetFractalOctaves(5);
    Chunk::noise.SetFractalLacunarity(1.75);
    Chunk::noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    Chunk::noise.SetFractalType(FastNoiseLite::FractalType_FBm);
}

void Chunk::GetVertices(VP::VPModel::Builder &builder) {
    std::unordered_map<VPModel::Vertex, uint32_t> uniqueVertices{};
    std::vector<VPModel::Vertex> vertices;

    for (auto& blockRow : blocks) {
        for (auto& blockColumn : blockRow) {
            for (auto& block : blockColumn) {
                if (block.id != 0) {
                    vertices.clear();
                    Chunk::GetVertices(vertices, block);
                    for (auto& vertex : vertices) {
                        vertex.position += position;
                        if (uniqueVertices.count(vertex) == 0) {
                            uniqueVertices[vertex] = static_cast<uint32_t>(builder.vertices.size());
                            builder.vertices.push_back(vertex);
                        }
                        builder.indices.push_back(uniqueVertices[vertex]);
                    }
                }
            }
        }
    }
}

Block &Chunk::GetBlock(uint8_t x, uint8_t y, uint8_t z) {
    return blocks[y][x][z];
}

void Chunk::GetVertices(std::vector<VP::VPModel::Vertex> &vertices, const Block& block) {

    vertices = {
            {block.position + glm::vec3{-.5f, -.5f, -.5f} , {.9f, .9f, .9f}},
            {block.position + glm::vec3{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {block.position + glm::vec3{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {block.position + glm::vec3{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {block.position + glm::vec3{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {block.position + glm::vec3{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {block.position + glm::vec3{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {block.position + glm::vec3{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {block.position + glm::vec3{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {block.position + glm::vec3{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {block.position + glm::vec3{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {block.position + glm::vec3{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {block.position + glm::vec3{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {block.position + glm::vec3{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {block.position + glm::vec3{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {block.position + glm::vec3{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {block.position + glm::vec3{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {block.position + glm::vec3{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {block.position + glm::vec3{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {block.position + glm::vec3{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {block.position + glm::vec3{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {block.position + glm::vec3{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {block.position + glm::vec3{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {block.position + glm::vec3{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {block.position + glm::vec3{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {block.position + glm::vec3{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {block.position + glm::vec3{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {block.position + glm::vec3{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {block.position + glm::vec3{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {block.position + glm::vec3{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {block.position + glm::vec3{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {block.position + glm::vec3{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {block.position + glm::vec3{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {block.position + glm::vec3{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {block.position + glm::vec3{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {block.position + glm::vec3{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    };
}

