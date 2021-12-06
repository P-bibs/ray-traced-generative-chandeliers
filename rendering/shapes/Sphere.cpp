#include "Sphere.h"
#include "SimpleRectangle.h"
#include "SimpleTriangle.h"
#include <memory>

Sphere::Sphere(int param1, int param2) {
    int verticalDivisions = std::max(2, param1);
    int radialDivisions = std::max(3, param2);

    float PI = M_PI;

    auto surfaces = std::vector<std::unique_ptr<Surface>>();

    for (int i = 0; i < radialDivisions; i++) {
        float theta1 = (static_cast<float>(i) / radialDivisions) * 2 * PI;
        float theta2 = (static_cast<float>(i + 1) / radialDivisions) * 2 * PI;

        for (int j = 0; j < verticalDivisions; j++) {
            float phi1 = (static_cast<float>(j) / verticalDivisions) * PI;
            float phi2 = (static_cast<float>(j + 1) / verticalDivisions) * PI;

            // Create strip of clementine peels. The top and bottom are triangles, and the middle is
            // rectangles
            if (j == 0) {
                // Find coordinates for bottom triangle
                glm::vec3 topRightSpherical = glm::vec3(0.5, phi2, theta2);
                glm::vec3 topLeftSpherical = glm::vec3(0.5, phi2, theta1);
                glm::vec3 bottomSpherical = glm::vec3(0.5, 0, 0);

                glm::vec3 topRight = sphericalToCartesian(topRightSpherical);
                glm::vec3 topLeft = sphericalToCartesian(topLeftSpherical);
                glm::vec3 bottom = sphericalToCartesian(bottomSpherical);

                glm::vec3 topRightNormal = glm::normalize(topRight);
                glm::vec3 topLeftNormal = glm::normalize(topLeft);
                glm::vec3 bottomNormal = glm::normalize(bottom);

                surfaces.push_back(std::make_unique<SimpleTriangle>(SimpleTriangle(
                    topRight, topLeft, bottom, 1, topRightNormal, topLeftNormal, bottomNormal)));

            } else if (j == verticalDivisions - 1) {
                // Find coordinates for top triangle
                glm::vec3 bottomLeftSpherical = glm::vec3(0.5, phi1, theta1);
                glm::vec3 bottomRightSpherical = glm::vec3(0.5, phi1, theta2);
                glm::vec3 topSpherical = glm::vec3(0.5, PI, 0);

                glm::vec3 bottomLeft = sphericalToCartesian(bottomLeftSpherical);
                glm::vec3 bottomRight = sphericalToCartesian(bottomRightSpherical);
                glm::vec3 top = sphericalToCartesian(topSpherical);

                glm::vec3 bottomLeftNormal = glm::normalize(bottomLeft);
                glm::vec3 bottomRightNormal = glm::normalize(bottomRight);
                glm::vec3 topNormal = glm::normalize(top);

                surfaces.push_back(
                    std::make_unique<SimpleTriangle>(SimpleTriangle(bottomLeft,
                                                                    bottomRight,
                                                                    top,
                                                                    1,
                                                                    bottomLeftNormal,
                                                                    bottomRightNormal,
                                                                    topNormal)));
            } else {
                // Find coordinates for main body rectangles
                glm::vec3 bottomLeftSpherical = glm::vec3(0.5, phi1, theta1);
                glm::vec3 bottomRightSpherical = glm::vec3(0.5, phi1, theta2);
                glm::vec3 topRightSpherical = glm::vec3(0.5, phi2, theta2);
                glm::vec3 topLeftSpherical = glm::vec3(0.5, phi2, theta1);

                glm::vec3 bottomLeft = sphericalToCartesian(bottomLeftSpherical);
                glm::vec3 bottomRight = sphericalToCartesian(bottomRightSpherical);
                glm::vec3 topRight = sphericalToCartesian(topRightSpherical);
                glm::vec3 topLeft = sphericalToCartesian(topLeftSpherical);

                glm::vec3 bottomLeftNormal = glm::normalize(bottomLeft);
                glm::vec3 bottomRightNormal = glm::normalize(bottomRight);
                glm::vec3 topRightNormal = glm::normalize(topRight);
                glm::vec3 topLeftNormal = glm::normalize(topLeft);

                surfaces.push_back(
                    std::make_unique<SimpleTriangle>(SimpleTriangle(bottomLeft,
                                                                    bottomRight,
                                                                    topRight,
                                                                    1,
                                                                    bottomLeftNormal,
                                                                    bottomRightNormal,
                                                                    topRightNormal)));

                surfaces.push_back(std::make_unique<SimpleTriangle>(SimpleTriangle(bottomLeft,
                                                                                   topRight,
                                                                                   topLeft,
                                                                                   1,
                                                                                   bottomLeftNormal,
                                                                                   topRightNormal,
                                                                                   topLeftNormal)));
            }
        }
    }

    // append surfaces together
    for (auto &surface : surfaces) {
        std::vector<GLfloat> surfaceTris = surface->getVerticesAndNormals();
        m_vertexData.insert(m_vertexData.begin(), surfaceTris.begin(), surfaceTris.end());
    }

    /**
     * Initialize the OpenGLShapeProperties so that the shape is ready to be drawn.
     * Note that this does not actually draw the shape, that happens in ShapesScene.cpp
     */
    initializeOpenGLShapeProperties();
}

// Convert spherical coordinates to cartesian coordinates
glm::vec3 Sphere::sphericalToCartesian(glm::vec3 spherical) {
    float r = spherical.x;
    float phi = spherical.y;
    float theta = spherical.z;

    float x = r * sin(phi) * cos(theta);
    float y = r * cos(phi);
    float z = r * sin(phi) * sin(theta);

    return glm::vec3(x, y, z);
}
