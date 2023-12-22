#pragma once
#include <queue>

#include "KeyboardInput.h"
#include <map>
#include <unordered_map>
#include <functional>
#include <string>
using Callback = std::function<void()>;
using MouseCallback = std::function<void(glm::vec2)>;

class VPInputManager {

public:
    VPInputManager(GLFWwindow* context_window);
    ~VPInputManager() {};
    //Delete copy constructor and assignment operator
    VPInputManager(const VPInputManager&) = delete;
    VPInputManager& operator=(const VPInputManager&) = delete;

    /* Keyboard */
    void AddKeyboardCallback(int key, KeyboardAction action, const std::string& name, Callback callback);
    void RemoveKeyboardCallback(int key, KeyboardAction action, const std::string& name);
    void QueueKeyboardInput(const KeyboardInput& input);

    /* Mouse */
    void AddMouseCallback(int key, MouseAction action, const std::string& name, const MouseCallback& callback);
    void RemoveMouseCallback(int key, MouseAction action, const std::string& name);
    void QueueMouseInput(const MouseInput& input);

    void ProcessInput();

    bool IsKeyPressed(int key);
    bool IsMouseKeyPressed(int key);
private:
    void Reset();

    const KeyboardInput& UnqueueKeyboardInput();
    const MouseInput& UnqueueMouseInput();

public:
    std::unordered_map<int, std::map<std::string , Callback>> * GetKeyboardCallbackFromAction(KeyboardAction action);
    std::unordered_map<int, std::map<std::string , MouseCallback>> * GetMouseCallbackFromAction(MouseAction action);

private:
    /* Keyboard */
    std::unordered_map<int, std::map<std::string , Callback>> callbacks_keyboard_pressed;
    std::unordered_map<int, std::map<std::string , Callback>> callbacks_keyboard_release;
    std::unordered_map<int, std::map<std::string , Callback>> callbacks_keyboard_repeat;


    /* Mouse */
    std::unordered_map<int, std::map<std::string , MouseCallback>> callbacks_mouse_pressed;
    std::unordered_map<int, std::map<std::string , MouseCallback>> callbacks_mouse_release;
    //TODO
    std::unordered_map<int, std::map<std::string , MouseCallback>> callbacks_mouse_moved;

    std::queue<KeyboardInput> keyboardInputs;
    std::queue<MouseInput> MouseInputs;

    GLFWwindow* window;
};
