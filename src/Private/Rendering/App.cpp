
#include "Rendering/App.h"
#define GLFW_INCLUDE_VULKAN

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

#include "GLFW/glfw3.h"
#include "glm/gtc/constants.hpp"
#include <stdexcept>
#include <array>

#include "Rendering/VPGameObject.h"
#include "Rendering/VPSimpleRenderSystem.h"



void VP::App::run() {
    VPSimpleRenderSystem simpleRenderSystem{Device, Renderer.GetSwapChainRenderPass()};
    camera.SetViewDirection(glm::vec3(0.f), glm::vec3(0.0f, 0.f, 1.f));
    while(!Window.ShouldClose()) {
        glfwPollEvents();
        float aspect = Renderer.GetAspectRatio();
        //camera.SetOrthographicProjection(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
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
    loadGameObjects();
    run();
}

VP::App::~App() {

}

void VP::App::loadGameObjects() {
    glm::vec3 color = {1.0f, 0.0f, 0.0f};

    std::shared_ptr<VPModel> model = createCubeModel(Device, {0.0f, 0.0f, 0.f});

    auto cube = VPGameObject::create();
    cube.model = model;
    cube.transform.translation = {.0f, .0f, 2.5f};
    cube.transform.scale = {.5f, .5f, .5f};
    gameObjects.push_back(std::move(cube));

}

std::unique_ptr<VP::VPModel> VP::App::createCubeModel(VP::VPEngineDevice& device, glm::vec3 offset) {
    std::vector<VP::VPModel::Vertex> vertices{

            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},



    };
    for (auto& v : vertices) {
        v.position += offset;
    }
    return std::make_unique<VP::VPModel>(device, vertices);
}









