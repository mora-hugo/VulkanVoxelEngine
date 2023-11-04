cmake_minimum_required(VERSION 3.16)
project(vulkantest)

set(CMAKE_CXX_STANDARD 17)
find_package(Vulkan REQUIRED)

add_subdirectory(lib/glfw-3.3.8)
SET(GLM_TEST_ENABLE OFF CACHE BOOL "GLM Build unit tests")
add_subdirectory(lib/glm      EXCLUDE_FROM_ALL)

include_directories("${PROJECT_SOURCE_DIR}/src/Public")


add_executable(${PROJECT_NAME} main.cpp
        src/Private/App/App.cpp
        src/Private/App/Window/VPWindow.cpp
        src/Private/Graphics/Pipeline/VPPipeline.cpp
        src/Private/App/Utils/VPEngineDevice.cpp
        src/Private/Graphics/Pipeline/VPEngineSwapChain.cpp
        src/Private/Graphics/VertexBuffer/VPModel.cpp
        src/Public/Graphics/VertexBuffer/VPModel.h
)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} Vulkan::Vulkan glm::glm)