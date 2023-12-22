
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

    inputManager->AddMouseCallback(GLFW_MOUSE_BUTTON_LEFT, MouseAction::VP_MOUSE_PRESSED, "ez2", [=](glm::vec2 pos) {
        previousMousePosition = pos;
    });
    inputManager->AddMouseCallback(NO_KEY, MouseAction::VP_MOUSE_MOVE, "ez", [=](glm::vec2 pos) {
        if(!inputManager->IsMouseKeyPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            return;
        }
        glm::vec2 delta = pos - previousMousePosition;
        previousMousePosition = pos;
        target->transform.rotation.y = glm::mod(target->transform.rotation.y+(delta.x/100), glm::two_pi<float>());
        target->transform.rotation.x = glm::mod(target->transform.rotation.x-(delta.y/100), glm::two_pi<float>());
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
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    target->transform.translation += upDir * 0.01f;

}

void VPPlayerController::MoveDown() {
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    target->transform.translation -= upDir * 0.01f;

}
