

#include "World/World.h"

Block *World::GetBlockWorld(int x, int y, int z) {

}

void World::GenerateWorld() {
    rootChunk = Chunk::Build({0,0,0});
    GenerateSimblings(rootChunk);

}

std::shared_ptr<Chunk> World::GenerateChunk(glm::vec3 position) {
    return Chunk::Build(position);
}

void World::GenerateSimblings(std::shared_ptr<Chunk>& chunk) {
    chunk->Left = GenerateChunk(chunk->position + glm::vec3{-Chunk::DEPTH, 0, 0});
    chunk->Right = GenerateChunk(chunk->position + glm::vec3{Chunk::DEPTH, 0, 0});
    chunk->Front = GenerateChunk(chunk->position + glm::vec3{0, 0, Chunk::DEPTH});
    chunk->Back = GenerateChunk(chunk->position + glm::vec3{0, 0, -Chunk::DEPTH});
}

void World::GetVertices(std::vector<VP::VPModel::Builder>& builders) {
    VP::VPModel::Builder builder {};
    rootChunk->GetVertices(builder);
    builders.push_back(builder);

    /* Make same to root chunks simblings */
    if(rootChunk->Left) {
        VP::VPModel::Builder builder1 {};
        rootChunk->Left->GetVertices(builder1);
        builders.push_back(builder1);
    }
    if(rootChunk->Right) {
        VP::VPModel::Builder builder2 {};
        rootChunk->Right->GetVertices(builder2);
        builders.push_back(builder);
    }
    if(rootChunk->Front) {
        VP::VPModel::Builder builder3 {};
        rootChunk->Front->GetVertices(builder3);
        builders.push_back(builder3);
    }
    if(rootChunk->Back) {
        VP::VPModel::Builder builder4 {};
        rootChunk->Back->GetVertices(builder4);
        builders.push_back(builder4);
    }



}
