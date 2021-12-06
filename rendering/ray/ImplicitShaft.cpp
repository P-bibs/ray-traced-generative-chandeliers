#include "ImplicitShaft.h"

ImplicitShaft::ImplicitShaft() {}

IntersectResult ImplicitShaft::intersectRay(glm::vec4 P, glm::vec4 d) {

    // Calculate the intersection of the ray with an infinite cylinder of r=0.5
    float A = d.x * d.x + d.z * d.z;
    float B = 2 * P.x * d.x + 2 * P.z * d.z;
    float C = P.x * P.x + P.z * P.z - 0.25;
    float discriminant = B * B - 4 * A * C;

    auto intersections = std::vector<IntersectResult>();

    // If discriminant < 0, no real solutions
    if (discriminant >= 0) {
        // Calculate first intersection point and add data to vector
        float t1 = (-B + sqrt(discriminant)) / (2 * A);
        auto t1CollisionPoint = P + (t1 * d);
        if (t1CollisionPoint.y < 0.5 && t1CollisionPoint.y > -0.5) {
            float theta = atan2(t1CollisionPoint.z, t1CollisionPoint.x);
            float u = theta < 0 ? -theta / (2 * PI) : 1 - (theta / (2 * PI));
            float v = t1CollisionPoint.y + 0.5;
            intersections.push_back(
                {t1, glm::normalize(glm::vec3(t1CollisionPoint.x, 0, t1CollisionPoint.z)), u, v});
        }

        // Calculate second intersection point and add data to vector
        float t2 = (-B - sqrt(discriminant)) / (2 * A);
        auto t2CollisionPoint = P + (t2 * d);
        if (t2CollisionPoint.y < 0.5 && t2CollisionPoint.y > -0.5) {
            float theta = atan2(t2CollisionPoint.z, t2CollisionPoint.x);
            float u = theta < 0 ? -theta / (2 * PI) : 1 - (theta / (2 * PI));
            float v = t2CollisionPoint.y + 0.5;
            intersections.push_back(
                {t2, glm::normalize(glm::vec3(t2CollisionPoint.x, 0, t2CollisionPoint.z)), u, v});
        }
    }

    // Return sentinel value if no intersections, otherwise the closest non-negative intersection
    if (intersections.size() == 0) {
        return {std::nullopt, glm::vec3(0, 0, 0)};
    } else {
        return IntersectResult::closestIntersection(intersections);
    }
}
