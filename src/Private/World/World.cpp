

#include <iostream>
#include "World/World.h"
#include "JobSystem.h"
/* Structures used to pass data to the job */
struct GenerateWorldJobData {
    std::unordered_map<glm::vec2, std::shared_ptr<Chunk>>* chunks = nullptr;
    int x = 0;
    int z = 0;

};
struct GetVerticesJobData {
    std::shared_ptr<Chunk> chunk = nullptr;
    std::vector<VP::VPModel::Builder>* builders = nullptr;
    int i = 0;
};

std::shared_ptr<Block> World::GetBlockWorld(int x, int y, int z) {
    auto chunk = GetChunkAtCoords({x,y,z});
    if(!chunk) return nullptr;

    return chunk->GetBlock(x - chunk->position.x, y - chunk->position.y, z - chunk->position.z);
}

void World::GenerateWorld() {
    const size_t threads = std::thread::hardware_concurrency();
    JobSystem jobSystem(threads-1, 65536);
    Job* parent = jobSystem.CreateEmptyJob();

    for (int x = 0; x < 20; x++) {
        for (int z = 0; z < 20; z++) {
            auto* data = new GenerateWorldJobData(); // Crée une nouvelle instance de GenerateWorldJobData
            data->chunks = &chunks;
            data->x = x;
            data->z = z;
            Job *job = jobSystem.CreateJobAsChild(Job_BuildChunk, parent, (void *) data);
            jobSystem.Run(job);
        }
    }

    jobSystem.Run(parent);
    jobSystem.Wait(parent);

}
std::shared_ptr<Chunk> World::GenerateChunk(glm::vec3 position) {
    return Chunk::Build(position);
}

void World::GetVertices(std::vector<VP::VPModel::Builder>& builders) {
    const size_t threads = std::thread::hardware_concurrency();
    builders.resize(chunks.size());
    std::cout << "chunks size : " << chunks.size() << std::endl;
    JobSystem jobSystem(threads-1, 65536);
    Job* parent = jobSystem.CreateEmptyJob();
    int i = 0;
    for(auto& chunk : chunks) {
        auto  data = new GetVerticesJobData();
        data->chunk = chunk.second;
        data->builders = &builders;
        data->i = i;
        Job *job = jobSystem.CreateJobAsChild(Job_GetVertices, parent, (void *) data);
        jobSystem.Run(job);
        i++;

    }
    jobSystem.Run(parent);
    jobSystem.Wait(parent);

}


std::shared_ptr<Chunk> World::GetChunkAtCoords(glm::vec3 position) {
    glm::vec2 chunkPosition {0};
    chunkPosition.x = (int)std::floor(position.x / Chunk::DEPTH);
    chunkPosition.y = (int)std::floor(position.z / Chunk::DEPTH);
    return chunks.count(chunkPosition) > 0 ? chunks.at(chunkPosition) : nullptr;
}

void World::Job_BuildChunk(void *data) {
    auto* myData = (GenerateWorldJobData*)data;
    myData->chunks->insert({glm::vec2({myData->x,myData->z}), Chunk::Build({myData->x*Chunk::DEPTH,0,myData->z*Chunk::DEPTH})});
    delete myData; //TODO
}

void World::Job_GetVertices(void *data) {
    auto * myData = (GetVerticesJobData *)data;
    std::vector<VP::VPModel::Builder>* builders = myData->builders;
    myData->chunk->GetVertices(builders->at(myData->i));
    delete myData; //TODO
}

