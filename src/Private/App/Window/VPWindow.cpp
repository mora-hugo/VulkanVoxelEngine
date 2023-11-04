//
// Created by hugom on 28/10/2023.
//

#include "App/Window/VPWindow.h"
#include <stdexcept>

void VP::VPWindow::InitWindow() {
    glfwInit();
    // Remove opengl context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Remove window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    Window = glfwCreateWindow(Width,Height,WindowName.c_str(), nullptr,nullptr);

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
