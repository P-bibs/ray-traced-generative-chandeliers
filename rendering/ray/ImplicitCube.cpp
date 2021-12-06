#include "ImplicitCube.h"

ImplicitCube::ImplicitCube()
    : m_topPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0, 0.5, 0), glm::vec3(0, 0.5, 0))))
    , m_bottomPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0, -0.5, 0), glm::vec3(0, -0.5, 0))))
    , m_leftPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(-0.5, 0, 0), glm::vec3(-0.5, 0, 0))))
    , m_rightPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0.5, 0, 0), glm::vec3(0.5, 0, 0))))
    , m_frontPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0, 0, 0.5), glm::vec3(0, 0, 0.5))))
    , m_backPlane(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0, 0, -0.5), glm::vec3(0, 0, -0.5)))) {}

IntersectResult ImplicitCube::intersectRay(glm::vec4 P, glm::vec4 d) {
    auto intersections = std::vector<IntersectResult>();

    // Calculate intersection with top plane. If within bounds, return
    // the intersection and normal.
    IntersectResult topIntersection = m_topPlane->intersectRay(P, d);
    if (topIntersection.tValue.has_value()) {
        glm::vec4 intersectionPoint = P + d * topIntersection.tValue.value();
        if (intersectionPoint.x >= -0.5 && intersectionPoint.x <= 0.5 &&
            intersectionPoint.z >= -0.5 && intersectionPoint.z <= 0.5) {
            topIntersection.u = intersectionPoint.x + 0.5;
            topIntersection.v = 1 - (intersectionPoint.z + 0.5);
            intersections.push_back(topIntersection);
        }
    }
    // Calculate intersection with bottom plane
    IntersectResult bottomIntersection = m_bottomPlane->intersectRay(P, d);
    if (bottomIntersection.tValue.has_value()) {
        glm::vec4 intersectionPoint = P + d * bottomIntersection.tValue.value();
        if (intersectionPoint.x >= -0.5 && intersectionPoint.x <= 0.5 &&
            intersectionPoint.z >= -0.5 && intersectionPoint.z <= 0.5) {
            bottomIntersection.u = intersectionPoint.x + 0.5;
            bottomIntersection.v = intersectionPoint.z + 0.5;
            intersections.push_back(bottomIntersection);
        }
    }

    // Calculate intersection with left plane
    IntersectResult leftIntersection = m_leftPlane->intersectRay(P, d);
    if (leftIntersection.tValue.has_value()) {
        glm::vec4 intersectionPoint = P + d * leftIntersection.tValue.value();
        if (intersectionPoint.y >= -0.5 && intersectionPoint.y <= 0.5 &&
            intersectionPoint.z >= -0.5 && intersectionPoint.z <= 0.5) {
            leftIntersection.u = intersectionPoint.z + 0.5;
            leftIntersection.v = intersectionPoint.y + 0.5;
            intersections.push_back(leftIntersection);
        }
    }
    // Calculate intersection with right plane
    IntersectResult rightIntersection = m_rightPlane->intersectRay(P, d);
    if (rightIntersection.tValue.has_value()) {
        glm::vec4 intersectionPoint = P + d * rightIntersection.tValue.value();
        if (intersectionPoint.y >= -0.5 && intersectionPoint.y <= 0.5 &&
            intersectionPoint.z >= -0.5 && intersectionPoint.z <= 0.5) {
            rightIntersection.u = 1 - (intersectionPoint.z + 0.5);
            rightIntersection.v = intersectionPoint.y + 0.5;
            intersections.push_back(rightIntersection);
        }
    }

    // Calculate intersection with front plane
    IntersectResult frontIntersection = m_frontPlane->intersectRay(P, d);
    if (frontIntersection.tValue.has_value()) {
        glm::vec4 intersectionPoint = P + d * frontIntersection.tValue.value();
        if (intersectionPoint.x >= -0.5 && intersectionPoint.x <= 0.5 &&
            intersectionPoint.y >= -0.5 && intersectionPoint.y <= 0.5) {
            frontIntersection.u = intersectionPoint.x + 0.5;
            frontIntersection.v = intersectionPoint.y + 0.5;
            intersections.push_back(frontIntersection);
        }
    }
    // Calculate intersection with back plane
    IntersectResult backIntersection = m_backPlane->intersectRay(P, d);
    if (backIntersection.tValue.has_value()) {
        glm::vec4 intersectionPoint = P + d * backIntersection.tValue.value();
        if (intersectionPoint.x >= -0.5 && intersectionPoint.x <= 0.5 &&
            intersectionPoint.y >= -0.5 && intersectionPoint.y <= 0.5) {
            backIntersection.u = 1 - (intersectionPoint.x + 0.5);
            backIntersection.v = intersectionPoint.y + 0.5;
            intersections.push_back(backIntersection);
        }
    }

    // Return sentinel value if no intersections, otherwise the closest non-negative intersection
    if (intersections.size() == 0) {
        return {std::nullopt, glm::vec3(0, 0, 0)};
    } else {
        return IntersectResult::closestIntersection(intersections);
    }
}
