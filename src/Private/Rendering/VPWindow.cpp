//
// Created by hugom on 28/10/2023.
//

#include "Rendering/VPWindow.h"
#include "Rendering/App.h"
#include <stdexcept>
#include <iostream>
#include <functional>


void VP::VPWindow::InitWindow(App* app) {
    this->AppRef = app;


}

VP::VPWindow::VPWindow(int width, int height, const std::string &windowName) : Width(width), Height(height), WindowName(windowName) {
    glfwInit();
    // Remove opengl context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Remove window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);


    Window = glfwCreateWindow(Width,Height,WindowName.c_str(), nullptr,nullptr);
    glfwSetWindowUserPointer(Window, this);


    /* Callbacks from GLFW */
    glfwSetFramebufferSizeCallback(Window, frameBufferResizedCallback);
    glfwSetKeyCallback(Window, key_callback);
    glfwSetMouseButtonCallback(Window, mouse_callback);
    glfwSetCursorPosCallback(Window, mouse_position_callback);
}

VP::VPWindow::~VPWindow() {
    glfwDestroyWindow(Window);
    glfwTerminate();
}

bool VP::VPWindow::ShouldClose() {
    return glfwWindowShouldClose(Window);
}

void VP::VPWindow::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(instance, Window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
}

VkExtent2D VP::VPWindow::getExtent() {
    return {static_cast<uint32_t>(Width), static_cast<uint32_t>(Height)};
}

bool VP::VPWindow::WasWindowResized() const {
    return frameBufferResized;
}

void VP::VPWindow::ResetWindowResized() {
    frameBufferResized = false;
}

void VP::VPWindow::frameBufferResizedCallback(GLFWwindow *window, int width, int height) {
    auto windowResized = reinterpret_cast<VPWindow*>(glfwGetWindowUserPointer(window));
    windowResized->frameBufferResized = true;
    windowResized->Width = width;
    windowResized->Height = height;



}

void VP::VPWindow::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    VPWindow *vp_window = static_cast<VPWindow*>(glfwGetWindowUserPointer(window));
    if(!vp_window) return;

    KeyboardInput input(key, scancode, action);
    vp_window->AppRef->GetInputManager().QueueKeyboardInput(input);

}

void VP::VPWindow::mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    VPWindow *vp_window = static_cast<VPWindow*>(glfwGetWindowUserPointer(window));
    if(!vp_window) return;

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    glm::vec2 mousePos {mouseX, mouseY};


    MouseInput input(button, action,mousePos);
    vp_window->AppRef->GetInputManager().QueueMouseInput(input);
}

void VP::VPWindow::mouse_position_callback(GLFWwindow *window, double xpos, double ypos) {
    VPWindow *vp_window = static_cast<VPWindow*>(glfwGetWindowUserPointer(window));
    if(!vp_window) return;

    glm::vec2 mousePos {xpos, ypos};

    //TODO
    MouseInput input(0, MouseInput::MOUSE_MOVE_INT, mousePos);
    vp_window->AppRef->GetInputManager().QueueMouseInput(input);
}
