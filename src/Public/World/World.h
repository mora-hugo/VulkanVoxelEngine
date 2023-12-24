#pragma once


#include <map>
#include "Chunk.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
class World {
public:

    Block * GetBlockWorld(int x, int y, int z);

    std::shared_ptr<Chunk> GetChunkAtCoords(glm::vec3 position);

    void GenerateWorld();
    std::unordered_map<glm::vec2, std::shared_ptr<Chunk>> chunks;




    void GetVertices(std::vector<VP::VPModel::Builder>& builders);

private:

    void GenerateSimblings(std::shared_ptr<Chunk>& chunk);
    std::shared_ptr<Chunk> GenerateChunk(glm::vec3 position);
    /* Chunk at 0,0,0 */

};


