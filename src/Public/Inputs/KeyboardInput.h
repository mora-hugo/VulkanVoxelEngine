#pragma once

#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"

#define NO_KEY -1


enum class KeyboardAction {
    VP_KEY_PRESSED,
    VP_KEY_RELEASED,
    VP_KEY_REPEAT,
    VP_KEY_UNKNOWN
};

enum class MouseAction {
    VP_MOUSE_PRESSED,
    VP_MOUSE_RELEASED,
    VP_MOUSE_REPEAT,
    VP_MOUSE_MOVE,
    VP_MOUSE_UNKNOWN
};


struct KeyboardInput {
public:
    KeyboardInput() = default;
    KeyboardInput(int key, int scancode, int action);
    KeyboardInput(const KeyboardInput&) = default;
    KeyboardInput &operator=(const KeyboardInput&) = delete;

private:
    static KeyboardAction GetKeyboardActionFromInt(int action);

public:
    int key = NO_KEY;
    int scan_code = NO_KEY;
    KeyboardAction action = KeyboardAction::VP_KEY_UNKNOWN;


};


struct MouseInput {
public:
    MouseInput() = default;
    MouseInput(int key, int action, const glm::vec2& position);
    MouseInput(const MouseInput&) = default;
    MouseInput &operator=(const MouseInput&) = delete;

private:
    static MouseAction GetMouseActionFromInt(int action);

public:
    int key = NO_KEY;
    glm::vec2 position = glm::vec2(0.0f);
    const static inline int MOUSE_MOVE_INT = 3;


    MouseAction action = MouseAction::VP_MOUSE_UNKNOWN;


};
