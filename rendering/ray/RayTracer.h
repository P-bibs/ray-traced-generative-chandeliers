#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "CS123SceneData.h"
#include "Canvas2D.h"
#include "GL/glew.h"
#include "ImplicitCone.h"
#include "ImplicitCube.h"
#include "ImplicitCylinder.h"
#include "ImplicitSphere.h"
#include "RGBA.h"
#include "RayScene.h"
#include "Scene.h"
#include "Settings.h"
#include <QtConcurrent/QtConcurrent>
#include <algorithm>
#include <geometric.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <tuple>

#define EPSILON 0.00001f
// Near plane distance
#define K 0.1f
#define REFLECTION_DEPTH 5


// Helper function to calculate the texture coordinate from U/V coordinate
inline int texIndex(float coord, int repeat, int size) { return static_cast<int>(coord * repeat * size) % size; };

// A struct to store the result of a ray tracing collision
struct TraceResult {
    TraceResult(IntersectResult i, glm::mat4 t, CS123ScenePrimitive p)
        : intersection(i)
        , transform(t)
        , primitive(p) {}
    // The intersection result
    IntersectResult intersection;
    // The transform of the collided object
    glm::mat4 transform;
    // The collided object
    CS123ScenePrimitive primitive;
};

class RayTracer {
  public:
    RayTracer();
    // trace a single pixel and return the color
    static std::optional<TraceResult>
    traceRayFromPoint(glm::vec4 P, glm::vec4 d, int depth, RayScene *scene);
    // check if there is an object between two points using ray tracing (useful for calculating
    // shadows from point lights)
    static bool checkOcclusionBetweenPoints(glm::vec3 point1, glm::vec3 point2, RayScene *scene);
    // check if there is an object starting at a point going infinitely in a direction using ray
    // tracing (useful for calculating shadows from directional lights)
    static bool checkOcclusionForRay(glm::vec3 point, glm::vec3 ray, RayScene *scene);
    // Ray trace a full scan line
    static bool traceScanLine(
        std::tuple<int, int, int, RGBA *, RGBAfloat *, RayScene *, Canvas2D *, float, float, glm::mat4> args);
    // Ray trace a full scan line with super sampling
    static bool traceScanLineSuperSampled(
        std::tuple<int, int, int, RGBA *, RGBAfloat *, RayScene *, Canvas2D *, float, float, glm::mat4> args);
    // Ray trace from a screen space point and return the color
    static RGBAfloat traceScreenSpacePoint(float u, float v, glm::mat4 cameraViewMatrix, RayScene *scene);

  private:
    static RGBAfloat calculateLightingEquation(std::optional<TraceResult> intersection,
                                          RayScene *scene,
                                          glm::vec4 P,
                                          glm::vec4 d,
                                          int depth);
};

#endif // RAYTRACER_H
