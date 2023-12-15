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
        bool WasWindowResized() const;
        void ResetWindowResized();

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR * surface);
    private:
        void InitWindow();

    private:
        static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        GLFWwindow * Window;

        int Width;
        int Height;
        bool frameBufferResized = false;

        const std::string WindowName;
    };


}

