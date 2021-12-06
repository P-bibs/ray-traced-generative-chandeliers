#include "ImplicitPlane.h"

#define EPSILON 0.000001

ImplicitPlane::ImplicitPlane(glm::vec3 point, glm::vec3 normal)
    : m_point(glm::vec4(point, 1))
    , m_normal(glm::vec4(normal, 0)) {}

IntersectResult ImplicitPlane::intersectRay(glm::vec4 P, glm::vec4 d) {

    // Don't count an intersection if the ray is parallel to the plane
    if (abs(glm::dot(m_normal, d)) < EPSILON) {
        return {std::nullopt, glm::vec3(0, 0, 0)};
    }

    // Implicit equation for plane:
    float t = (glm::dot(m_normal, m_point) - glm::dot(m_normal, P)) / glm::dot(m_normal, d);
    if (t >= 0) {
        return {std::optional<float>(t), glm::normalize(glm::vec3(m_normal))};
    } else {
        // can't determine u,v for an infinite plane. If this plane is being used as part of a
        // larger shape the uvs will be added later
        return {std::nullopt, glm::vec3(0, 0, 0), 0, 0};
    }
}

glm::vec4 ImplicitPlane::getNormal() { return m_normal; }
