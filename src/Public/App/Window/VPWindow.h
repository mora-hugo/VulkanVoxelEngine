#pragma once

#define GLFW_INCLUDE_VULKAN

#include <string>
#include "GLFW/glfw3.h"
namespace VP {
    class VPWindow {
    public:
        VPWindow(const int width, const int height, const std::string & windowName);
        VPWindow(const VPWindow&) = delete;
        VPWindow &operator=(const VPWindow&) = delete;

        ~VPWindow();

        bool ShouldClose();
        VkExtent2D getExtent();
        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR * surface);
    private:
        void InitWindow();

        GLFWwindow * Window;

        const int Width;
        const int Height;
        const std::string WindowName;
    };


}

