

#include <iostream>
#include "World/World.h"
#include "JobSystem.h"

Block *World::GetBlockWorld(int x, int y, int z) {
    return nullptr;
}

void World::GenerateWorld() {
    const size_t threads = std::thread::hardware_concurrency();
    JobSystem jobSystem(threads-1, 65536);
    Job* parent = jobSystem.CreateEmptyJob();

    for (int x = 0; x < 60; x++) {
        for (int z = 0; z < 60; z++) {
            MyJobData* data = new MyJobData(); // Crée une nouvelle instance de MyJobData
            data->chunks = &chunks;
            data->x = x;
            data->z = z;
            Job *job = jobSystem.CreateJobAsChild(MakeJob, parent, (void*)data);
            jobSystem.Run(job);
        }
    }

    jobSystem.Run(parent);
    jobSystem.Wait(parent);

}
std::shared_ptr<Chunk> World::GenerateChunk(glm::vec3 position) {
    return Chunk::Build(position);
}

void World::GenerateSimblings(std::shared_ptr<Chunk>& chunk) {

}
struct MyJobData2 {
    std::shared_ptr<Chunk> chunk;
    std::vector<VP::VPModel::Builder>* builders;
    int i = 0;
};
void World::GetVertices(std::vector<VP::VPModel::Builder>& builders) {
    const size_t threads = std::thread::hardware_concurrency();
    builders.resize(chunks.size());
    std::cout << "chunks size : " << chunks.size() << std::endl;
    JobSystem jobSystem(threads-1, 65536);
    Job* parent = jobSystem.CreateEmptyJob();
    int i = 0;
    for(auto& chunk : chunks) {
        MyJobData2* data = new MyJobData2(); // Crée une nouvelle instance de MyJobData
        data->chunk = chunk.second;
        data->builders = &builders;
        data->i = i;
        Job *job = jobSystem.CreateJobAsChild(MakeJob2, parent, (void*)data);
        jobSystem.Run(job);
        i++;

    }
    jobSystem.Run(parent);
    jobSystem.Wait(parent);




}
void World::MakeJob2(void *data) {
    MyJobData2* myData = (MyJobData2*)data;
    std::vector<VP::VPModel::Builder>* builders = myData->builders;
    myData->chunk->GetVertices(builders->at(myData->i));
    delete myData;
}

std::shared_ptr<Chunk> World::GetChunkAtCoords(glm::vec3 position) {
    return nullptr;
}

void World::MakeJob(void *data) {
    MyJobData* myData = (MyJobData*)data;
    myData->chunks->insert({glm::vec2({myData->x,myData->z}), Chunk::Build({myData->x*Chunk::DEPTH,0,myData->z*Chunk::DEPTH})});
    delete myData;

}

