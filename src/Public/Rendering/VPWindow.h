#pragma once

#define GLFW_INCLUDE_VULKAN

#include <string>
#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"


namespace VP {
    class App;
    class VPWindow {
    public:
        VPWindow(int width, int height, const std::string &windowName);
        VPWindow(const VPWindow&) = delete;
        VPWindow &operator=(const VPWindow&) = delete;

        ~VPWindow();

        GLFWwindow * GetGLFWWindow() const { return Window; }
        bool ShouldClose();
        VkExtent2D getExtent();
        bool WasWindowResized() const;
        void ResetWindowResized();

        void CreateWindowSurface(VkInstance instance, VkSurfaceKHR * surface);
        void InitWindow(VP::App* app);

    private:

    private:
        static void frameBufferResizedCallback(GLFWwindow* window, int width, int height);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
        static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
        GLFWwindow * Window;
        App* AppRef;

        int Width;
        int Height;
        bool frameBufferResized = false;

        const std::string WindowName;
    };


}

