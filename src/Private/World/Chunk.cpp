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
    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();
    chunk->position = position;
    for (size_t x = 0; x < Chunk::DEPTH; ++x) {
            for (size_t z = 0; z < Chunk::DEPTH; ++z) {
                    for(size_t y = 0; y < Chunk::HEIGHT; ++y) {
                        chunk->blocks[y][x][z] = std::make_shared<Block>();
                        chunk->blocks[y][x][z]->position = {x,y,z};
                        chunk->blocks[y][x][z]->parent_chunk = chunk;

                        int noiseValue = static_cast<int>((std::clamp<float>(-1,1,Chunk::noise.GetNoise((float)x+position.x , (float)z+position.z)) *100+100));
                        if(y > noiseValue) {
                            chunk->blocks[y][x][z]->id = 1;
                        }
                        else {
                            chunk->blocks[y][x][z]->id = 0;
                        }


                    }


            }
    }
    return std::move(chunk);
}

Chunk::Chunk() {
    Chunk::noise.SetFractalOctaves(5);
    Chunk::noise.SetSeed(13347);
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
                if (block && block->id != 0) {
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

std::shared_ptr<Block> Chunk::GetBlock(uint8_t x, uint8_t y, uint8_t z) {
    if(x >= Chunk::DEPTH || y >= Chunk::HEIGHT || z >= Chunk::DEPTH || x < 0 || y < 0 || z < 0) {
        return nullptr;
    }
    return blocks[y][x][z];
}

void Chunk::GetVertices(std::vector<VP::VPModel::Vertex> &vertices, const std::shared_ptr<Block>& block) {


    std::shared_ptr<Block> topBlock = block->parent_chunk->GetBlock((int)block->position.x , (int)block->position.y-1, (int)block->position.z);
    std::shared_ptr<Block> bottomBlock = block->parent_chunk->GetBlock((int)block->position.x , (int)block->position.y+1, (int)block->position.z);
    std::shared_ptr<Block> leftBlock = block->parent_chunk->GetBlock((int)block->position.x-1 , (int)block->position.y, (int)block->position.z);
    std::shared_ptr<Block> rightBlock = block->parent_chunk->GetBlock((int)block->position.x+1 , (int)block->position.y, (int)block->position.z);
    std::shared_ptr<Block> frontBlock = block->parent_chunk->GetBlock((int)block->position.x , (int)block->position.y, (int)block->position.z+1);
    std::shared_ptr<Block> backBlock = block->parent_chunk->GetBlock((int)block->position.x , (int)block->position.y, (int)block->position.z-1);


    if(!rightBlock || (rightBlock && rightBlock->id == 0)) {

        //right
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, .5f}, {.8f, .8f, .1f}});
    }


    //top
    if(!topBlock || (topBlock && topBlock->id == 0)) {
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, .5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, .5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, -.5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, .5f}, {.9f, .6f, .1f}});
    }



    //left
    if(!leftBlock || (leftBlock && leftBlock->id == 0)) {
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, .5f}, {.8f, .8f, .1f}});
    }

    //bottom
    if(!bottomBlock || (bottomBlock && bottomBlock->id == 0)) {
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, -.5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, .5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, .5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, -.5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, -.5f}, {.9f, .6f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, .5f}, {.9f, .6f, .1f}});
    }

    //front
    if(!frontBlock || (frontBlock && frontBlock->id == 0)) {
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, .5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, .5f}, {.8f, .8f, .1f}});
    }

    //back
    if(!backBlock || (backBlock && backBlock->id == 0)) {
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, .5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{-.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, -.5f, -.5f}, {.8f, .8f, .1f}});
        vertices.push_back({block->position + glm::vec3{.5f, .5f, -.5f}, {.8f, .8f, .1f}});
    }


}

