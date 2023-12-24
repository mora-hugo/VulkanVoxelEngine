

#include <iostream>
#include "World/World.h"

Block *World::GetBlockWorld(int x, int y, int z) {
    return nullptr;
}

void World::GenerateWorld() {
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            std::cout << "Generating chunk at " << x << " " << z << std::endl;
            chunks[glm::vec2({x,z})] = Chunk::Build({x*Chunk::DEPTH,0,z*Chunk::DEPTH});

        }
    }




}

std::shared_ptr<Chunk> World::GenerateChunk(glm::vec3 position) {
    return Chunk::Build(position);
}

void World::GenerateSimblings(std::shared_ptr<Chunk>& chunk) {

}

void World::GetVertices(std::vector<VP::VPModel::Builder>& builders) {
    for(auto& chunk : chunks) {
        VP::VPModel::Builder builder {};
        chunk.second->GetVertices(builder);
        builders.push_back(builder);

    }


}

std::shared_ptr<Chunk> World::GetChunkAtCoords(glm::vec3 position) {

}
