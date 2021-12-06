#include "ImplicitCylinder.h"

ImplicitCylinder::ImplicitCylinder()
    : m_shaft(std::make_unique<ImplicitShaft>(ImplicitShaft()))
    , m_topPlane(
          std::make_unique<ImplicitPlane>(ImplicitPlane(glm::vec3(0, 0.5, 0), glm::vec3(0, 1, 0))))
    , m_bottomPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0, -0.5, 0), glm::vec3(0, -1, 0)))) {}

IntersectResult ImplicitCylinder::intersectRay(glm::vec4 P, glm::vec4 d) {
    auto intersections = std::vector<IntersectResult>();

    // Calculate collision with infinite cylinder and throw out if too high or low
    auto shaftIntersection = m_shaft->intersectRay(P, d);
    if (shaftIntersection.tValue.has_value()) {
        auto shaftCollisionTValue = shaftIntersection.tValue.value();
        auto shaftCollisionPoint = P + (shaftCollisionTValue * d);
        if (shaftCollisionPoint.y <= 1 && shaftCollisionPoint.y >= -1) {
            intersections.push_back(shaftIntersection);
        }
    }

    // Calculate intersection with bottom plane and throw out if not within radius
    auto topPlaneIntersection = m_topPlane->intersectRay(P, d);
    if (topPlaneIntersection.tValue.has_value()) {
        auto topPlaneCollisionTValue = topPlaneIntersection.tValue.value();
        auto topPlaneCollisionPoint = P + (topPlaneCollisionTValue * d);
        if (pow(topPlaneCollisionPoint.x, 2) + pow(topPlaneCollisionPoint.z, 2) <= 0.25) {
            topPlaneIntersection.u = topPlaneCollisionPoint.x + 0.5;
            topPlaneIntersection.v = 1 - (topPlaneCollisionPoint.z + 0.5);
            intersections.push_back(topPlaneIntersection);
        }
    }

    // Calculate intersection with bottom plane and throw out if not within radius
    auto bottomPlaneIntersection = m_bottomPlane->intersectRay(P, d);
    if (bottomPlaneIntersection.tValue.has_value()) {
        auto bottomPlaneCollisionTValue = bottomPlaneIntersection.tValue.value();
        auto bottomPlaneCollisionPoint = P + (bottomPlaneCollisionTValue * d);
        if (pow(bottomPlaneCollisionPoint.x, 2) + pow(bottomPlaneCollisionPoint.z, 2) <= 0.25) {
            bottomPlaneIntersection.u = bottomPlaneCollisionPoint.x + 0.5;
            bottomPlaneIntersection.v = bottomPlaneCollisionPoint.z + 0.5;
            intersections.push_back(bottomPlaneIntersection);
        }
    }

    // Return sentinel value if no intersections, otherwise the closest non-negative intersection
    if (intersections.size() == 0) {
        return {std::nullopt, glm::vec3(0, 0, 0)};
    } else {
        return IntersectResult::closestIntersection(intersections);
    }
}
