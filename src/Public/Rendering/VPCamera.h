#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

namespace VP {
    class VPCamera {
    public:
        //Corresponding to the box
        void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);

        void SetPerspectiveProjection(float fov, float aspect, float near, float far);

        const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }

        const glm::mat4& GetViewMatrix() const { return viewMatrix; }

        void SetViewDirection(const glm::vec3 &position, const glm::vec3& direction, const glm::vec3& up = {0.f, -1.f, 0.f});

        void SetViewTarget(const glm::vec3 &position, const glm::vec3& target, const glm::vec3& up = {0.f, -1.f, 0.f});

        void SetViewYXZ(const glm::vec3 &position, const glm::vec3& rotation);

        void AddPosition(const glm::vec3& position);

    private:
        glm::mat4 projectionMatrix {1.f};
        glm::mat4 viewMatrix {1.f};
    };
}
