#pragma once
#include "GLFW/glfw3.h"
#include "Inputs/VPInputManager.h"
#include "Rendering/VPGameObject.h"


namespace VP {
    class VPPlayerController {
    public:
        void BindCallbacks(VPGameObject * target, VPInputManager *inputManager);
    private:
        void MoveForward();

        void MoveBackward();

        void MoveLeft();

        void MoveRight();

        void MoveUp();

        void MoveDown();

        GLFWwindow* window;
        VPGameObject * target;
        glm::vec2 previousMousePosition;
    };

}


