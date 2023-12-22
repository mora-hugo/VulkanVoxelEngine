
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




#include "Rendering/VPGameObject.h"
#include "Rendering/VPSimpleRenderSystem.h"
#include "Inputs/VPPlayerController.h"
#include "tiny_obj_loader.h"





void VP::App::run() {
    camera.SetViewDirection(glm::vec3(0.f), glm::vec3(0.0f, 1.f, 0.0f));

    auto ViewerObject = VPGameObject::create();
    VPPlayerController playerController{};
    playerController.BindCallbacks(&ViewerObject, &inputManager);




    while(!Window.ShouldClose()) {
        glfwPollEvents();
        float aspect = Renderer.GetAspectRatio();
        //camera.SetOrthographicProjection(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        // Process input and call callbacks
        inputManager.ProcessInput();

        camera.SetViewYXZ(ViewerObject.transform.translation, ViewerObject.transform.rotation);



        if(auto commandBuffer = Renderer.BeginFrame()) {

            //Begin offscreen shadow pass
            //Render shadow casting objects
            //End offscreen shadow pass
            Renderer.BeginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);


            Renderer.EndSwapChainRenderPass(commandBuffer);

            Renderer.EndFrame();



        }
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
    const auto cubemodel = VPModel::Builder::createModelFromFile(Device,"../models/colored_cube.obj");
    for(int i = 0; i < 10000; ++i) {

        auto cube = VPGameObject::create();
        cube.model = cubemodel;
        cube.transform.translation = {0.0f, 0.0f, 1.f*i};
        cube.transform.scale = {.5f, .5f, .5f};
        cube.transform.rotation = {0.0f, 0.0f, 0.0f};
        cube.color = color;
        gameObjects.push_back(std::move(cube));

    }




}












