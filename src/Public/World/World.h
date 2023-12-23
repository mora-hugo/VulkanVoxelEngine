#pragma once


#include "Chunk.h"

class World {
public:

    Block * GetBlockWorld(int x, int y, int z);

    void GenerateWorld();
    std::shared_ptr<Chunk> rootChunk;


    void GetVertices(std::vector<VP::VPModel::Builder>& builders);

private:

    void GenerateSimblings(std::shared_ptr<Chunk>& chunk);
    std::shared_ptr<Chunk> GenerateChunk(glm::vec3 position);
    /* Chunk at 0,0,0 */

};


