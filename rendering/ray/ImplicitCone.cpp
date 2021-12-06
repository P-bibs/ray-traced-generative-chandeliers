#include "ImplicitCone.h"

ImplicitCone::ImplicitCone()
    : m_bottomFace(std::make_unique<ImplicitPlane>(
          ImplicitPlane(glm::vec3(0, -0.5, 0), glm::vec3(0, -1, 0)))) {}

IntersectResult ImplicitCone::intersectRay(glm::vec4 P, glm::vec4 d) {
    auto intersections = std::vector<IntersectResult>();

    // Calculate collision with infinite cone via quadratic equation
    float A = d.x * d.x + d.z * d.z - 0.25 * (d.y * d.y);
    float B = 2 * P.x * d.x + 2 * P.z * d.z - 0.5 * (P.y * d.y) + 0.25 * d.y;
    float C = P.x * P.x + P.z * P.z - 0.25 * P.y * P.y + 0.25 * P.y - 0.0625;
    float discriminant = B * B - 4 * A * C;
    // No real solutions if the discriminant is negative
    if (discriminant >= 0) {
        // Calculate the first intersection point
        {
            float t1 = (-B + sqrt(discriminant)) / (2 * A);
            auto t1CollisionPoint = P + (t1 * d);
            // Throw out points that aren't within our unit cone
            if (t1CollisionPoint.y < 0.5 && t1CollisionPoint.y > -0.5) {
                // The normal of the cone must have slope=2
                auto flatNormal =
                    glm::normalize(glm::vec3(t1CollisionPoint.x, 0, t1CollisionPoint.z));
                auto normal = glm::normalize(glm::vec3(flatNormal.x, 0.5, flatNormal.z));

                float theta = atan2(t1CollisionPoint.z, t1CollisionPoint.x);
                float u = theta < 0 ? -theta / (2 * PI) : 1 - (theta / (2 * PI));
                float v = t1CollisionPoint.y + 0.5;

                intersections.push_back({t1, normal, u, v});
            }
        }
        // Calculate the second intersection point
        {
            float t2 = (-B - sqrt(discriminant)) / (2 * A);
            auto t2CollisionPoint = P + (t2 * d);
            // Throw out points that aren't within our unit cone
            if (t2CollisionPoint.y < 0.5 && t2CollisionPoint.y > -0.5) {
                // The normal of the cone must have slope=2
                auto flatNormal =
                    glm::normalize(glm::vec3(t2CollisionPoint.x, 0, t2CollisionPoint.z));
                auto normal = glm::normalize(glm::vec3(flatNormal.x, 0.5, flatNormal.z));

                float theta = atan2(t2CollisionPoint.z, t2CollisionPoint.x);
                float u = theta < 0 ? -theta / (2 * PI) : 1 - (theta / (2 * PI));
                float v = t2CollisionPoint.y + 0.5;

                intersections.push_back({t2, normal, u, v});
            }
        }
    }

    // Calculate intersection with bottom plane
    auto bottomFaceCollision = m_bottomFace->intersectRay(P, d);
    if (bottomFaceCollision.tValue.has_value()) {
        auto bottomFaceCollisionTValue = bottomFaceCollision.tValue.value();
        auto bottomFaceCollisionPoint = P + (bottomFaceCollisionTValue * d);

        // Throw out intersection if not within bottom circle
        if (pow(bottomFaceCollisionPoint.x, 2) + pow(bottomFaceCollisionPoint.z, 2) <= 0.25) {
            bottomFaceCollision.u = bottomFaceCollisionPoint.x + 0.5;
            bottomFaceCollision.v = bottomFaceCollisionPoint.z + 0.5;

            intersections.push_back(bottomFaceCollision);
        }
    }

    // Return sentinel value if no intersections, otherwise the closest non-negative intersection
    if (intersections.size() == 0) {
        return {std::nullopt, glm::vec3(0, 0, 0)};
    } else {
        return IntersectResult::closestIntersection(intersections);
    }
}
