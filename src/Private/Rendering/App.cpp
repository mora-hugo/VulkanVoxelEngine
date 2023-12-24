
#include "Rendering/App.h"
#include "Inputs/VPInputManager.h"
#define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

#include "GLFW/glfw3.h"
#include "glm/gtc/constants.hpp"
#include <stdexcept>
#include <array>
#include <iostream>
#include <chrono>




#include "Rendering/VPGameObject.h"
#include "Rendering/VPSimpleRenderSystem.h"
#include "Inputs/VPPlayerController.h"
#include "tiny_obj_loader.h"
#include "World/Chunk.h"
#include "World/World.h"


void VP::App::run() {
    camera.SetViewDirection(glm::vec3(0.f), glm::vec3(0.0f, 1.f, 0.0f));

    auto ViewerObject = VPGameObject::create();
    VPPlayerController playerController{};
    playerController.BindCallbacks(&ViewerObject, &inputManager);
    auto currentTimestamp = std::chrono::high_resolution_clock::now();
    auto previousTimestamp = std::chrono::high_resolution_clock::now();
    auto deltaTime = 0.f;



    while(!Window.ShouldClose()) {
        glfwPollEvents();
        float aspect = Renderer.GetAspectRatio();
        //camera.SetOrthographicProjection(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 200.f);

        // Process input and call callbacks

        camera.SetViewYXZ(ViewerObject.transform.translation, ViewerObject.transform.rotation);

        inputManager.ProcessInput();

        if(auto commandBuffer = Renderer.BeginFrame()) {

            //Begin offscreen shadow pass
            //Render shadow casting objects
            //End offscreen shadow pass
            Renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);


            Renderer.EndSwapChainRenderPass(commandBuffer);

            Renderer.EndFrame();



        }
        deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTimestamp - previousTimestamp).count();
        previousTimestamp = currentTimestamp;
        currentTimestamp = std::chrono::high_resolution_clock::now();
        glfwSetWindowTitle(Window.GetGLFWWindow(), std::to_string(((int)1.f/deltaTime)).c_str());
    }

    // wait all ressources to be free
    vkDeviceWaitIdle(Device.device());
}

VP::App::App() {
    Window.InitWindow(this);

    loadGameObjects();
    run();
}

VP::App::~App() {
}

void VP::App::loadGameObjects() {
    glm::vec3 color = {1.0f, 0.0f, 0.0f};
    /* Generate Chunk */
    world.GenerateWorld();
    std::vector<VPModel::Builder> builders;
    world.GetVertices(builders);
    for(auto& builder : builders) {
        VPGameObject gameObject = VPGameObject::create();
        gameObject.transform.translation = {0.f, 0.f,  0.f};
        gameObject.transform.scale = {0.2f, 0.2f, 0.2f};
        gameObject.transform.rotation = {0.f, 0.f, 0.f};
        gameObject.model = std::make_shared<VPModel>(Device,builder);
        gameObject.color = color;
        std::cout << "Vertex count: " << builder.vertices.size() << std::endl;
        gameObjects.push_back(std::move(gameObject));
    }







}












