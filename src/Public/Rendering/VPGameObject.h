#pragma once

#include "VPModel.h"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

namespace VP {
struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.f, 1.f, 1.f};
    glm::vec3 rotation{};
    //Rotation use trait-bryan angles y,x,z
    glm::mat4 mat4() {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
                {
                        scale.x * (c1 * c3 + s1 * s2 * s3),
                                scale.x * (c2 * s3),
                                               scale.x * (c1 * s2 * s3 - c3 * s1),
                                                              0.0f,
                },
                {
                        scale.y * (c3 * s1 * s2 - c1 * s3),
                                scale.y * (c2 * c3),
                                               scale.y * (c1 * c3 * s2 + s1 * s3),
                                                              0.0f,
                },
                {
                        scale.z * (c2 * s1),
                                scale.z * (-s2),
                                               scale.z * (c1 * c2),
                                                              0.0f,
                },
                {translation.x, translation.y, translation.z, 1.0f}};
    }

};
class VPGameObject {
public:
    using id_t = unsigned int;

    static VPGameObject create() {
        static id_t nextId = 0;

        return VPGameObject{nextId++};
    }

    VPGameObject(const VPGameObject&) = delete;
    VPGameObject &operator=(const VPGameObject&) = delete;
    VPGameObject(VPGameObject&&) = default;
    VPGameObject &operator=(VPGameObject&&) = default;

    id_t getId() const {
        return id;
    }

    std::shared_ptr<VPModel> model;
    glm::vec3 color;
    TransformComponent transform;

private:
    VPGameObject(id_t id) : id(id) {}

    id_t id;
};
}


