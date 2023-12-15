#define MY_DEBUG

#define GLFW_INCLUDE_VULKAN



#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include "Rendering/App.h"


using namespace VP;

int main() {


    try {
        App app;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}