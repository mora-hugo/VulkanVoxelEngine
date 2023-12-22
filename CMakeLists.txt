cmake_minimum_required(VERSION 3.16)
project(vulkantest)

set(CMAKE_CXX_STANDARD 17)
find_package(Vulkan REQUIRED)


add_subdirectory(lib/glfw-3.3.8)
set(GLM_TEST_ENABLE OFF CACHE BOOL "GLM Build unit tests")
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)


set(TINY_OBJ_LOADER ./lib/tinyobjloader)
include_directories(${TINY_OBJ_LOADER})


include_directories("${PROJECT_SOURCE_DIR}/src/Public")

add_executable(${PROJECT_NAME}
        main.cpp
        src/Private/Rendering/App.cpp
        src/Private/Rendering/VPWindow.cpp
        src/Private/Rendering/VPPipeline.cpp
        src/Private/Rendering/VPEngineDevice.cpp
        src/Private/Rendering/VPEngineSwapChain.cpp
        src/Private/Rendering/VPModel.cpp
        src/Private/Rendering/VPRendering.cpp
        src/Private/Rendering/VPSimpleRenderSystem.cpp
        src/Private/Rendering/VPCamera.cpp
        src/Private/Inputs/KeyboardInput.cpp
        src/Private/Inputs/VPInputManager.cpp
        src/Private/Inputs/VPPlayerController.cpp
        src/Private/Rendering/VPUtils.cpp
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} Vulkan::Vulkan glm::glm)


if(WIN32)
    add_custom_target(run_custom_command ALL
            COMMAND cmd /c compile.bat
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running custom command")
    add_dependencies(run_custom_command ${PROJECT_NAME})
endif()
