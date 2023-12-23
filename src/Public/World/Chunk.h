#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include "Rendering/VPModel.h"
#include "Rendering/VPUtils.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/vec3.hpp"

using block_id_t = uint16_t;

struct Block {
    block_id_t id = 0;
    glm::vec3 position {0};
};
struct Chunk {
public:
    static constexpr int HEIGHT = 256;
    static constexpr int DEPTH = 16;

    // X Z Y
    std::array<std::array<std::array<Block,DEPTH>,DEPTH>,HEIGHT> blocks;

    Block& GetBlock(uint8_t x, uint8_t y, uint8_t z);

    static Chunk Build();

    static void GetVertices(std::vector<VP::VPModel::Vertex>& vertices, const Block& block);

    void GetVertices(VP::VPModel::Builder& builder);

private:
    Chunk();

};


