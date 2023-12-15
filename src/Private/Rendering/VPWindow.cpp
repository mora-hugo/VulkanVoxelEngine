//
// Created by hugom on 28/10/2023.
//

#include "Rendering/VPWindow.h"
#include <stdexcept>
#include <iostream>
#include <functional>

void VP::VPWindow::InitWindow() {
    glfwInit();
    // Remove opengl context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Remove window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    Window = glfwCreateWindow(Width,Height,WindowName.c_str(), nullptr,nullptr);
    glfwSetWindowUserPointer(Window, this);
    glfwSetFramebufferSizeCallback(Window, frameBufferResizedCallback);
    glfwSetKeyCallback(Window, key_callback);
    glfwSetCharModsCallback(Window, key_callback_unicode);

}

VP::VPWindow::VPWindow(const int width, const int height, const std::string &windowName) : Width(width), Height(height), WindowName(windowName){
    InitWindow();
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
    std::cout << key << std::endl;
}
