#include "ImplicitSphere.h"
#include "Settings.h"

ImplicitSphere::ImplicitSphere() {}

IntersectResult ImplicitSphere::intersectRay(glm::vec4 P, glm::vec4 d) {

    // Calculate quadratic coefficients
    float A = glm::dot(d, d);
    float B = 2 * glm::dot(d, P);
    // Account for homogenous coordinates by subtracting 1 from dot product
    float C = glm::dot(P, P) - 0.25 - 1;
    float discriminant = B * B - 4 * A * C;

    // If discriminant is greater than 0, there are up to two real solutions.
    if (discriminant < EPSILON) {
        return {std::nullopt, glm::vec3(0, 0, 0)};
    } else {
        // Collect intersections in a vector
        std::vector<IntersectResult> intersections = std::vector<IntersectResult>();

        {
            float t1 = (-B + sqrt(discriminant)) / (2 * A);
            glm::vec3 intersectionPoint = glm::vec3(P + t1 * d);

            float theta = atan2(intersectionPoint.z, intersectionPoint.x);
            float u = theta < 0 ? -theta / (2 * PI) : 1 - (theta / (2 * PI));
            // float v = std::abs(intersectionPoint.y - 0.5) < EPSILON ||
            //                   std::abs(intersectionPoint.y + 0.5) < EPSILON
            //               ? 0.5f
            //               : (asin(intersectionPoint.y / 0.5f) / PI) + 0.5f;
            float v = (asin(intersectionPoint.y / 0.5f) / PI) + 0.5f;

            intersections.push_back({std::optional<float>(t1), intersectionPoint, u, v});
        }

        {
            float t2 = (-B - sqrt(discriminant)) / (2 * A);
            glm::vec3 intersectionPoint = (glm::vec3(P + t2 * d));

            float theta = atan2(intersectionPoint.z, intersectionPoint.x);
            float u = theta < 0 ? -theta / (2 * PI) : 1 - (theta / (2 * PI));
            // float v = std::abs(intersectionPoint.y - 0.5) < EPSILON ||
            //                   std::abs(intersectionPoint.y + 0.5) < EPSILON
            //               ? 0.5f
            //               : (asin(intersectionPoint.y / 0.5f) / PI) + 0.5f;
            float v = (asin(intersectionPoint.y / 0.5f) / PI) + 0.5f;

            intersections.push_back({std::optional<float>(t2), intersectionPoint, u, v});
        }

        // Return the closest intersection
        return IntersectResult::closestIntersection(intersections);
    }
}
