#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <vector>

#define EPSILON 0.00001
#define PI 3.14159265358979323846

// An interect result represents the result of casting a ray to collide with an object.
// If there is no collision then tValue is equal to std::nullopt and accessing the `normal` is
// undefined. If there is a collision, an intersection occurs at the point P + t * d. `normal` is
// the object space normal at that point.
struct IntersectResult {
    // Static method to compute the closest non-negative intersection of a collection of
    // intersections
    static IntersectResult closestIntersection(std::vector<IntersectResult> &intersections) {
        // Filter out invalid intersections and return early if no valid intersections
        // are left
        std::vector<IntersectResult> validIntersections;
        validIntersections.reserve(intersections.size());
        for (int i = 0; i < intersections.size(); i++)
        {
            IntersectResult curr = intersections.at(i);
            if (curr.tValue.has_value() && curr.tValue.value() >= 0 + EPSILON) {
                validIntersections.push_back(curr);
            }
        }
        if (validIntersections.size() == 0) {
            return {std::nullopt, glm::vec3(0, 0, 0)};
        }

        // Find the closest intersection of the remaining intersections
        IntersectResult closestIntersection = validIntersections[0];
        for (auto intersection : validIntersections) {
            if (intersection.tValue.value() < closestIntersection.tValue.value()) {
                closestIntersection = intersection;
            }
        }
        return closestIntersection;
    }

    std::optional<float> tValue;
    glm::vec3 normal;
    float u;
    float v;
};

// A class that extends `Intersectable` must have an `intersectRay` method that implements
// raycasting and collision point detection functionality.
class Intersectable {
  public:
    Intersectable();

    // Computes the collision result of casting a ray `d` from point `P`. Does it hit this
    // object?
    virtual IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) = 0;
};

#endif // INTERSECTABLE_H
