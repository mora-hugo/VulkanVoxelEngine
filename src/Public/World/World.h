#pragma once


#include <map>
#include "Chunk.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
class World {

public:

    std::shared_ptr<Block> GetBlockWorld(int x, int y, int z);

    std::shared_ptr<Chunk> GetChunkAtCoords(glm::vec3 position);

    void GenerateWorld();
    std::unordered_map<glm::vec2, std::shared_ptr<Chunk>> chunks;




    void GetVertices(std::vector<VP::VPModel::Builder>& builders);

private:
    /* Jobs functions */
    static void Job_BuildChunk(void* data);
    static void Job_GetVertices(void* data);


    std::shared_ptr<Chunk> GenerateChunk(glm::vec3 position);


};


