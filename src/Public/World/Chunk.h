#pragma once

#include <array>
#include <cstdint>
#include <vector>
#include "Rendering/VPModel.h"
#include "Rendering/VPUtils.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/vec3.hpp"
#include "../../../lib/fast-noise-lite/FastNoiseLite.h"

using block_id_t = uint16_t;
struct Face {
    std::array<glm::vec3, 6> vertices;
};
struct Block {
    const static inline uint8_t TOP_FACE = 0;
    const static inline uint8_t BOTTOM_FACE = 1;
    const static inline uint8_t LEFT_FACE = 2;
    const static inline uint8_t RIGHT_FACE = 3;
    const static inline uint8_t FRONT_FACE = 4;
    const static inline uint8_t BACK_FACE = 5;

    block_id_t id = 0;
    glm::vec3 position {0};
    std::array<Face, 6> faces;

    std::shared_ptr<class Chunk> parent_chunk;
};
struct Chunk {
    //TODO make unique need a public constructor ??
    Chunk();

    /* FastNoiseLite noise instanciation */
    static inline FastNoiseLite noise;

    /* Y */
    static constexpr int HEIGHT = 128;

    /* X , Z */
    static constexpr int DEPTH = 16;

    // [X][Z][Y]
    std::array<std::array<std::array<std::shared_ptr<Block>,DEPTH>,DEPTH>,HEIGHT> blocks {std::make_shared<Block>()};

    std::shared_ptr<Block> GetBlock(uint8_t x, uint8_t y, uint8_t z);

    static std::shared_ptr<Chunk> Build(const glm::vec3& position);

    glm::vec3 position {0};



    void GetVertices(VP::VPModel::Builder& builder);

private:


    static void GetVertices(std::vector<VP::VPModel::Vertex>& vertices, const std::shared_ptr<Block>& block);







};


