#include "RayTracer.h"

#include "Canvas2D.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

RayTracer::RayTracer() {}

std::optional<TraceResult>
RayTracer::traceRayFromPoint(glm::vec4 P, glm::vec4 d, int depth, RayScene *scene) {

    // keep track of all intersections with objects
    auto intersections = std::vector<TraceResult>();

    // Loop through all shapes and calculate intersections
    for (std::tuple<CS123ScenePrimitive, glm::mat4x4> &primitiveAndTransform :
         scene->getPrimitives()) {

        auto primitive = std::get<0>(primitiveAndTransform);
        // A transformation matrix from object space to world space
        auto transform = std::get<1>(primitiveAndTransform);

        // Transformation matrix from world space to object space
        auto transformInverse = glm::inverse(transform);

        // Get object space versions of P and d
        auto objectSpaceP = transformInverse * P;
        auto objectSpaceD = transformInverse * d;

        // Figure out what kind of shape we're intersecting
        std::unique_ptr<Intersectable> shape;
        if (primitive.type == PrimitiveType::PRIMITIVE_CUBE) {
            shape = std::make_unique<ImplicitCube>(ImplicitCube());
        } else if (primitive.type == PrimitiveType::PRIMITIVE_CONE) {
            shape = std::make_unique<ImplicitCone>(ImplicitCone());
        } else if (primitive.type == PrimitiveType::PRIMITIVE_CYLINDER) {
            shape = std::make_unique<ImplicitCylinder>(ImplicitCylinder());
        } else if (primitive.type == PrimitiveType::PRIMITIVE_SPHERE) {
            shape = std::make_unique<ImplicitSphere>(ImplicitSphere());
        } else {
            continue;
        }
        // Calculate intersection in object space
        auto intersectResult = shape->intersectRay(objectSpaceP, objectSpaceD);

        // If there was no intersection, move on to the next shape
        if (!intersectResult.tValue.has_value()) {
            continue;
        }
        // If there was an intersection, add it to the list along with it's transform and primitive
        intersections.push_back(TraceResult(intersectResult, transform, primitive));
    }

    // If there were no intersections, none. Otherwise, return the intersection
    if (intersections.size() == 0) {
        return std::nullopt;
    } else {

        TraceResult closestIntersection = intersections[0];
        for (auto intersection : intersections) {
            if (intersection.intersection.tValue.value() <
                closestIntersection.intersection.tValue.value()) {
                closestIntersection = intersection;
            }
        }
        return std::optional<TraceResult>(closestIntersection);
    }
}

bool RayTracer::checkOcclusionBetweenPoints(glm::vec3 point1, glm::vec3 point2, RayScene *scene) {
    // Calculate the ray from point1 to point2
    glm::vec3 d = glm::normalize(point2 - point1);

    // Perform the ray trace (starting at point1) to check for intersections
    std::optional<TraceResult> traceResult = RayTracer::traceRayFromPoint(
        glm::vec4(point1 + EPSILON * 100.0f * d, 1), glm::vec4(d, 0), 0, scene);

    // If any object is intersected, then there is an obstacle between point1 and point2. However,
    // we have to make sure that the t value of the intersection is less than that of point2,
    // otherwise the object isn't actually between point1 and point2
    if (traceResult.has_value()) {
        float tValueOfPoint2 = glm::length(point2 - point1);
        if (traceResult.value().intersection.tValue.value() < tValueOfPoint2) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool RayTracer::checkOcclusionForRay(glm::vec3 point, glm::vec3 ray, RayScene *scene) {
    glm::vec3 d = glm::normalize(ray);

    // Perform the ray trace (starting at `point`) to check for intersections
    std::optional<TraceResult> traceResult = RayTracer::traceRayFromPoint(
        glm::vec4(point + EPSILON * 100.0f * d, 1), glm::vec4(d, 0), 0, scene);

    // If we hit an object, then there is an obstacle starting at point and going towards ray
    if (traceResult.has_value()) {
        return true;
    } else {
        return false;
    }
}

RGBAfloat RayTracer::calculateLightingEquation(std::optional<TraceResult> traceResult,
                                          RayScene *scene,
                                          glm::vec4 P,
                                          glm::vec4 d,
                                          int depth) {

    // If there is no intersection, return black
    if (traceResult == std::nullopt) {
        if(!scene->getEnvironmentMap().isUsed) {
            return RGBAfloat(0.0, 0.0, 0.0, 1.0);
        } else {
            IntersectResult interRes = ImplicitCube().intersectRay(glm::vec4(0),d);
            QImage img;

            //Implicit Assumption all sides are same dimensions
            int s = texIndex(interRes.u,1,scene->getEnvironmentMap().pos_x.width());
            int t = texIndex((1-interRes.v),1,scene->getEnvironmentMap().pos_x.height());

            //Decide Which Image is intersected
            glm::vec4 absD = glm::abs(d);
            if (absD.x > absD.y && absD.x > absD.z) {
                if (glm::sign(d.x) > 0) {
                    img = scene->getEnvironmentMap().pos_x;
                } else {
                    img = scene->getEnvironmentMap().neg_x;
                }
            } else if (absD.y > absD.x && absD.y > absD.z) {
                if (glm::sign(d.y) > 0) {
                    img = scene->getEnvironmentMap().pos_y;
                } else {
                    img = scene->getEnvironmentMap().neg_y;
                }
            } else {
                if (glm::sign(d.z) > 0) {
                    img = scene->getEnvironmentMap().pos_z;
                } else {
                    img = scene->getEnvironmentMap().neg_z;
                }
            }

            QColor pixel = img.pixel(s, t);
            return RGBAfloat(pixel.red()/255.0,
                             pixel.green()/255.0,
                             pixel.blue()/255.0,
                             1.0);
        }
    }
    TraceResult intersection = traceResult.value();

    // Transform the object space collision normal to world space.
    glm::vec3 normalInWorldCoords =
        glm::normalize(glm::inverse(glm::transpose(glm::mat3(intersection.transform))) *
                       intersection.intersection.normal);

    // Compute a vector from the world space intersection point to the world space camera
    glm::vec3 toEye = glm::vec3(-d);

    // Compute the world space intersection point
    glm::vec4 intersectionPoint = P + intersection.intersection.tValue.value() * d;

    // Extract global constants and material
    float ka = scene->getGlobalData().ka;
    float kd = scene->getGlobalData().kd;
    float ks = scene->getGlobalData().ks;
    float kt = scene->getGlobalData().kt;
    auto mat = intersection.primitive.material;

    // Initialize vector where we'll accumulate the lighting equation. Each color starts with the
    // value of it's ambient term
    glm::vec3 color = glm::vec3(ka * mat.cAmbient.r, ka * mat.cAmbient.g, ka * mat.cAmbient.b);

    // Loop through each light and calculate contribution to this pixel
    auto lights = scene->getLightData();
    for (auto light : lights) {

        // Calculate attenuation and light direction based on light type
        float attenuation;
        glm::vec3 toLightNormalized;
        if (light.type == LightType::LIGHT_POINT) {
            if (!settings.usePointLights) {
                continue;
            }

            // Ignore this light if it's occluded

             //TODO:TRANSP add transparency information into this
            if (settings.useShadows &&
                RayTracer::checkOcclusionBetweenPoints(
                    glm::vec3(intersectionPoint), glm::vec3(light.pos), scene)) {
                continue;
            }

            // calculate a vector from the intersection point to the light
            // and the length of that vector to find attenuation.
            glm::vec3 toLight = glm::vec3(light.pos - intersectionPoint);
            toLightNormalized = glm::normalize(toLight);
            float distance = glm::length(toLight);
            attenuation = std::min(1.0f,
                                   1.0f / (light.function.x + distance * light.function.y +
                                           distance * distance * light.function.z));

        } else if (light.type == LightType::LIGHT_DIRECTIONAL) {
            if (!settings.useDirectionalLights) {
                continue;
            }

            // Ignore this light if it's occluded
            //TODO:TRANSP add transparency information into this
            if (settings.useShadows && RayTracer::checkOcclusionForRay(glm::vec3(intersectionPoint),
                                                                       glm::vec3(-light.dir),
                                                                       scene)) {
                continue;
            }

            // Calculate a vector from the intersection point to the light
            toLightNormalized = glm::normalize(glm::vec3(-light.dir));
            // Don't attenuate directional lights
            attenuation = 1;
        } else {
            // Only handle point and directional lights.
            continue;
        }

        // Add diffuse contribution from this light, but only if the normal is facing towards the
        // light
        float cosTheta = glm::dot(toLightNormalized, normalInWorldCoords);
        if (cosTheta > 0) {
            glm::vec3 blended;
            if (settings.useTextureMapping && mat.textureMap.isUsed) {
                // Calculate point in texture map to take color from
                QImage *texture = mat.textureMap.handle;
                float u = intersection.intersection.u;
                float v = intersection.intersection.v;
                float j = mat.textureMap.repeatU;
                float k = mat.textureMap.repeatV;
                int w = texture->width();
                int h = texture->height();

                // perform actual calculation
                int s = texIndex(u,j,w);
                int t = texIndex((1-v),k,h);

                QColor pixel = texture->pixel(s, t);

                // convert from [0,255] to [0,1]
                glm::vec3 textureColor =
                    glm::vec3(pixel.red(), pixel.green(), pixel.blue()) / 255.0f;

                // Blend texture color with diffuse color
                blended = mat.blend * textureColor + (1 - mat.blend) * kd * glm::vec3(mat.cDiffuse);
            } else {
                // If there's no texture, then just use the diffuse color as the blended value
                blended = kd * glm::vec3(mat.cDiffuse);
            }

            color += attenuation * glm::vec3(light.color) * glm::vec3(blended) * cosTheta;
        }

        // Add specular contribution from this light, but only if the normal is facing towards the
        // light
        glm::vec3 reflectedLightRay =
            glm::normalize(glm::reflect(-toLightNormalized, normalInWorldCoords));
        float cosAlpha = glm::dot(reflectedLightRay, toEye);
        if (cosAlpha > 0) {
            // Use shininess as exponent on specular value
            float specularWithExponentApplied = glm::pow(cosAlpha, mat.shininess);

            // add specular contribution to final color
            color += attenuation * glm::vec3(light.color) * ks * glm::vec3(mat.cSpecular) *
                     specularWithExponentApplied;
        }
    }

    // Recursively trace ray to calculate reflection
    if (settings.useReflection && depth >= 1 &&
            glm::length(mat.cReflective) > 0.01) {
        // calculate reflected ray by reflecting around the normal
        glm::vec3 reflectedRay = glm::normalize(glm::reflect(glm::vec3(d), normalInWorldCoords));

        // The new ray starts at the intersection point but is offset by epsilon
        glm::vec4 newRayOriginPoint =
            intersectionPoint + (glm::vec4(reflectedRay, 0) * EPSILON * 10.0f);

        // trace the reflected ray
        auto result = RayTracer::traceRayFromPoint(
            newRayOriginPoint, glm::vec4(reflectedRay, 0), depth - 1, scene);

        // calculate lighting equation for reflection
        RGBAfloat reflectedColor = RayTracer::calculateLightingEquation(
            result, scene, newRayOriginPoint, glm::vec4(reflectedRay, 0), depth - 1);

        // add reflection to final color
        color += ks * glm::vec3(mat.cReflective) * (RGBAfloat::toVec(reflectedColor));
    }

    if (settings.useRefraction &&
            depth >= 1 &&
            glm::length(mat.cTransparent) > 0.01) {
        // test if we are on the inside or outside of the shape
        float ior = mat.ior;
        if ( glm::sign(glm::dot(normalInWorldCoords,toEye)) > 0) {
            normalInWorldCoords = normalInWorldCoords;
            ior = 1/ior;
        } else {
            normalInWorldCoords = -1.0f * normalInWorldCoords;
        }
        // calculate perpendicular component to normal vector along toEye

        glm::vec3 toEyePerpendicularToNormal = glm::normalize(toEye - normalInWorldCoords *
                                                                      glm::dot(normalInWorldCoords,toEye));

        // compute refracted angle inside material
        float reflectedAngle = glm::asin(ior * glm::abs(glm::length(glm::cross(glm::normalize(toEye),normalInWorldCoords))));

        // compute refreacted ray
        glm::vec3 refractedRay = glm::cos(reflectedAngle) * -normalInWorldCoords +
                                 glm::sin(reflectedAngle)* -toEyePerpendicularToNormal;

        // compute new origin point
        glm::vec4 newRayOriginPoint =
                intersectionPoint + (glm::vec4(refractedRay, 0) * EPSILON * 10.0f);

        // trace the refracted ray
        auto result = RayTracer::traceRayFromPoint(
            newRayOriginPoint, glm::vec4(refractedRay, 0), depth - 1, scene);

        // compute lighting equation for refraction
        RGBAfloat refractedColor = RayTracer::calculateLightingEquation(
            result, scene, newRayOriginPoint, glm::vec4(refractedRay, 0), depth - 1);

        // add refraction to final color
        color += kt * glm::vec3(mat.cTransparent) * (RGBAfloat::toVec(refractedColor));
    }

    // clamp values to the range [0,inf]
    return RGBAfloat(std::max(0.0f, color.r),
                     std::max(0.0f, color.g),
                     std::max(0.0f, color.b),
                     1.0f);
}

// Given a point in screen space, calculate the color via ray tracing
RGBAfloat RayTracer::traceScreenSpacePoint(float u,
                                      float v,
                                      glm::mat4 cameraViewMatrix,
                                      RayScene *scene) {

    // Calculate the ray from the camera to the pixel
    glm::vec4 pixelCenter = glm::vec4(u, v, -K, 1);
    glm::vec4 eyePosition = glm::vec4(0, 0, 0, 1);
    glm::vec4 d = glm::normalize(pixelCenter - eyePosition);

    // Convert P and d from camera space to world space
    glm::vec4 worldSpaceEye = glm::inverse(cameraViewMatrix) * eyePosition;
    glm::vec4 worldSpaceD = glm::inverse(cameraViewMatrix) * d;

    // Trace the ray and return the color
    std::optional<TraceResult> result =
        RayTracer::traceRayFromPoint(worldSpaceEye, worldSpaceD, REFLECTION_DEPTH, scene);
    RGBAfloat ret = RayTracer::calculateLightingEquation(
        result, scene, worldSpaceEye, worldSpaceD, REFLECTION_DEPTH);
    return ret;
}

bool RayTracer::traceScanLine(
    std::tuple<int, int, int, RGBA *, RGBAfloat *, RayScene *, Canvas2D *, float, float, glm::mat4> args) {

    // Args have to be wrapped in a tuple so we can call this function via QTConcurrent::mapped
    int row = std::get<0>(args);
    int width = std::get<1>(args);
    int height = std::get<2>(args);
    RGBA *pix = std::get<3>(args);
    RGBAfloat *post_pix = std::get<4>(args);
    RayScene *scene = std::get<5>(args);
    Canvas2D *canvas = std::get<6>(args);
    float viewAngleHeight = std::get<7>(args);
    float viewAngleWidth = std::get<8>(args);
    glm::mat4 cameraViewMatrix = std::get<9>(args);

    // For each pixel in this scan line, calculate the color via ray tracing and store it in the
    // canvas
    for (int col = 0; col < width; col++) {
        RGBAfloat result = RayTracer::traceScreenSpacePoint(
            2 * K * tan(glm::radians(viewAngleWidth) / 2.0) * ((col + 0.5) / width - 0.5),
            2 * K * tan(glm::radians(viewAngleHeight) / 2.0) * (0.5 - ((row + 0.5) / height)),
            cameraViewMatrix,
            scene);
        pix[row * width + col] = result;
        post_pix[row * width + col] = result;
    }

    // Update canvas and process events to maintain interactivity
    canvas->update();
    QCoreApplication::processEvents();

    // return successfully
    return true;
}

bool RayTracer::traceScanLineSuperSampled(
    std::tuple<int, int, int, RGBA *, RGBAfloat *, RayScene *, Canvas2D *, float, float, glm::mat4> args) {

    // Args have to be wrapped in a tuple so we can call this function via QTConcurrent::mapped
    int row = std::get<0>(args);
    int width = std::get<1>(args);
    int height = std::get<2>(args);
    RGBA *pix = std::get<3>(args);
    RGBAfloat *post_pix = std::get<4>(args);
    RayScene *scene = std::get<5>(args);
    Canvas2D *canvas = std::get<6>(args);
    float viewAngleHeight = std::get<7>(args);
    float viewAngleWidth = std::get<8>(args);
    glm::mat4 cameraViewMatrix = std::get<9>(args);

    // For each pixel on this line, super sample by tracing four points within the pixel and
    // averaging the resulting RGBA
    for (int col = 0; col < width; col++) {
        RGBA topLeft = RayTracer::traceScreenSpacePoint(
            2 * K * tan(glm::radians(viewAngleWidth) / 2.0) * ((col + 0.15) / width - 0.5),
            2 * K * tan(glm::radians(viewAngleHeight) / 2.0) * (0.5 - ((row + 0.15) / height)),
            cameraViewMatrix,
            scene);
        RGBA topRight = RayTracer::traceScreenSpacePoint(
            2 * K * tan(glm::radians(viewAngleWidth) / 2.0) * ((col + 0.85) / width - 0.5),
            2 * K * tan(glm::radians(viewAngleHeight) / 2.0) * (0.5 - ((row + 0.15) / height)),
            cameraViewMatrix,
            scene);
        RGBA bottomLeft = RayTracer::traceScreenSpacePoint(
            2 * K * tan(glm::radians(viewAngleWidth) / 2.0) * ((col + 0.15) / width - 0.5),
            2 * K * tan(glm::radians(viewAngleHeight) / 2.0) * (0.5 - ((row + 0.85) / height)),
            cameraViewMatrix,
            scene);
        RGBA bottomRight = RayTracer::traceScreenSpacePoint(
            2 * K * tan(glm::radians(viewAngleWidth) / 2.0) * ((col + 0.85) / width - 0.5),
            2 * K * tan(glm::radians(viewAngleHeight) / 2.0) * (0.5 - ((row + 0.85) / height)),
            cameraViewMatrix,
            scene);

        float r = (topLeft.r + topRight.r + bottomLeft.r + bottomRight.r) / 4.0f;
        float g = (topLeft.g + topRight.g + bottomLeft.g + bottomRight.g) / 4.0f;
        float b = (topLeft.b + topRight.b + bottomLeft.b + bottomRight.b) / 4.0f;

        pix[row * width + col] = RGBA(r, g, b, 255);
    }

    // Update canvas and process events to maintain interactivity
    canvas->update();
    QCoreApplication::processEvents();

    // return successfully
    return true;
}
