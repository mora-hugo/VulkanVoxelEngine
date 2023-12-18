
#include <iostream>
#include "Inputs/VPPlayerController.h"

using namespace VP;
void VPPlayerController::BindCallbacks(VPGameObject * target, VPInputManager *inputManager) {
    this->target = target;
    inputManager->AddKeyboardCallback(GLFW_KEY_W, KeyboardAction::VP_KEY_REPEAT, "MoveForward", std::bind(&VPPlayerController::MoveForward, this));
    inputManager->AddKeyboardCallback(GLFW_KEY_S, KeyboardAction::VP_KEY_REPEAT, "MoveBackward", std::bind(&VPPlayerController::MoveBackward, this));
    inputManager->AddKeyboardCallback(GLFW_KEY_A, KeyboardAction::VP_KEY_REPEAT, "MoveLeft", std::bind(&VPPlayerController::MoveLeft, this));
    inputManager->AddKeyboardCallback(GLFW_KEY_D, KeyboardAction::VP_KEY_REPEAT, "MoveRight", std::bind(&VPPlayerController::MoveRight, this));

    inputManager->AddKeyboardCallback(GLFW_KEY_Q, KeyboardAction::VP_KEY_REPEAT, "MoveUp", std::bind(&VPPlayerController::MoveUp, this));
    inputManager->AddKeyboardCallback(GLFW_KEY_E, KeyboardAction::VP_KEY_REPEAT, "MoveDown", std::bind(&VPPlayerController::MoveDown, this));

    inputManager->AddMouseCallback(GLFW_MOUSE_BUTTON_LEFT, MouseAction::VP_MOUSE_MOVE, "RotateLeft", [](glm::vec2 position){
        std::cout << "x: " << position.x << " y: " << position.y << std::endl;
    });
}

void VPPlayerController::MoveForward() {
    float yaw = target->transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};

    target->transform.translation += forwardDir * 0.01f;
}

void VPPlayerController::MoveBackward() {
    float yaw = target->transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};

    target->transform.translation -= forwardDir * 0.01f;
}

void VPPlayerController::MoveLeft() {
    float yaw = target->transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};

    target->transform.translation -= rightDir * 0.01f;
}

void VPPlayerController::MoveRight() {
    float yaw = target->transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};

    target->transform.translation += rightDir * 0.01f;
}

void VPPlayerController::MoveUp() {
    float yaw = target->transform.rotation.y;
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    target->transform.translation += upDir * 0.01f;

}

void VPPlayerController::MoveDown() {
    float yaw = target->transform.rotation.y;
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    target->transform.translation -= upDir * 0.01f;

}
