//
// Created by hugom on 15/12/2023.
//

#include "Inputs/VPInputManager.h"

void VPInputManager::AddKeyboardCallback(int key, KeyboardAction action, const std::string& name, Callback callback) {
    auto callbacks = GetKeyboardCallbackFromAction(action);
    if(callbacks->find(key) == callbacks->end()) {
        callbacks->insert({});
    }
    (*callbacks)[key].insert({name, callback});
}

void VPInputManager::RemoveKeyboardCallback(int key, KeyboardAction action, const std::string& name) {
    auto callbacks = GetKeyboardCallbackFromAction(action);
    if(callbacks->find(key) == callbacks->end()) {
        return;
    }

    callbacks->erase(key);
}



void VPInputManager::Reset() {
    callbacks_keyboard_pressed.clear();
    callbacks_keyboard_release.clear();
    callbacks_keyboard_repeat.clear();
}

void VPInputManager::QueueKeyboardInput(const KeyboardInput &input) {
    keyboardInputs.push(input);
}

void VPInputManager::ProcessInput(GLFWwindow* window) {
    // Processing REPEATING keys (Input polling)
    for(std::pair<int, std::map<std::string , Callback>> pair : callbacks_keyboard_repeat) {
        if(glfwGetKey(window, pair.first) == GLFW_PRESS) {
            for(std::pair<std::string, Callback> callback : pair.second) {
                callback.second();
            }
        }
    }

    // Processing PRESSED AND RELEASED keys (From window callback)
    while(!keyboardInputs.empty()) {
        const KeyboardInput& input = UnqueueKeyboardInput();
        auto callbacks = GetKeyboardCallbackFromAction(input.action);
        if(callbacks->find(input.key) == callbacks->end()) {
            continue;
        }

        for(std::pair<std::string, Callback> callback : (*callbacks)[input.key]) {
            callback.second();
        }
    }

    // Processing mouse inputs
    while(!MouseInputs.empty()) {
        const MouseInput& input = UnqueueMouseInput();
        auto callbacks = GetMouseCallbackFromAction(input.action);

        if(!callbacks) continue;
        if(callbacks->find(input.key) == callbacks->end()) {
            continue;
        }

        for(std::pair<std::string, MouseCallback> callback : (*callbacks)[input.key]) {
            callback.second(input.position);
        }


    }

}

const KeyboardInput &VPInputManager::UnqueueKeyboardInput() {
    const KeyboardInput& input = keyboardInputs.front();
    keyboardInputs.pop();
    return input;
}

std::unordered_map<int, std::map<std::string, Callback>> *VPInputManager::GetKeyboardCallbackFromAction(KeyboardAction action) {
    switch (action) {
        case KeyboardAction::VP_KEY_PRESSED:
            return &callbacks_keyboard_pressed;
        case KeyboardAction::VP_KEY_RELEASED:
            return &callbacks_keyboard_release;
        case KeyboardAction::VP_KEY_REPEAT:
            return &callbacks_keyboard_repeat;
        case KeyboardAction::VP_KEY_UNKNOWN:
            break;
    }
    return nullptr;
}

void VPInputManager::AddMouseCallback(int key, MouseAction action, const std::string &name, const MouseCallback & callback) {

    auto callbacks = GetMouseCallbackFromAction(action);

    if(!callbacks) return;


    if(callbacks->find(key) == callbacks->end()) {
        callbacks->insert({});
    }
    if(action == MouseAction::VP_MOUSE_MOVE)
    {
        (*callbacks)[0].insert({name, callback});
    }
    (*callbacks)[key].insert({name, callback});
}

std::unordered_map<int, std::map<std::string, MouseCallback>> *
VPInputManager::GetMouseCallbackFromAction(MouseAction action) {
    switch (action) {
        case MouseAction::VP_MOUSE_PRESSED:
            return &callbacks_mouse_pressed;
        case MouseAction::VP_MOUSE_RELEASED:
            return &callbacks_mouse_release;
        case MouseAction::VP_MOUSE_MOVE:
            return &callbacks_mouse_moved;
        default:
            break;
    }
    return nullptr;
}

void VPInputManager::RemoveMouseCallback(int key, MouseAction action, const std::string &name) {
    auto callbacks = GetMouseCallbackFromAction(action);

    if(!callbacks) return;

    if(callbacks->find(key) == callbacks->end()) {
        return;
    }

    callbacks->erase(key);
}

const MouseInput &VPInputManager::UnqueueMouseInput() {
    const MouseInput& input = MouseInputs.front();
    MouseInputs.pop();
    return input;
}

void VPInputManager::QueueMouseInput(const MouseInput &input) {
    MouseInputs.push(input);
}
