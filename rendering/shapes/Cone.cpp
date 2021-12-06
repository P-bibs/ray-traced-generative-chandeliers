#include "Cone.h"
#include "Disc.h"
#include "IsocelesTriangle.h"
#include "SimpleTriangle.h"
#include "Square.h"
#include "math.h"
#include "vector"

Cone::Cone(int param1, int param2) {
    int verticalDivisions = std::max(1, param1);
    int radialDivisions = std::max(3, param2);

    float PI = M_PI;

    auto surfaces = std::vector<std::unique_ptr<Surface>>();

    // create the cone base (a disc)
    surfaces.push_back(std::make_unique<Disc>(
        Disc(glm::vec3(0, -0.5, 0), verticalDivisions, radialDivisions, false)));

    // create the cone sides (a series of triangles)
    glm::vec3 peak = glm::vec3(0, 0.5, 0);
    for (int i = 0; i < radialDivisions; i++) {
        // Divide the circular base up into a series of triangles
        auto rightPercentAroundCircle = static_cast<float>(i) / radialDivisions;
        auto leftPercentAroundCircle = static_cast<float>(i + 1) / radialDivisions;

        // Calculate the cartesian coordinates of the two points on the circle
        auto rightCoord = glm::vec3(0.5 * std::cos(rightPercentAroundCircle * 2 * PI),
                                    -0.5,
                                    0.5 * std::sin(rightPercentAroundCircle * 2 * PI));
        auto leftCoord = glm::vec3(0.5 * std::cos(leftPercentAroundCircle * 2 * PI),
                                   -0.5,
                                   0.5 * std::sin(leftPercentAroundCircle * 2 * PI));

        // calculate the normal vectors
        auto rightNormal = glm::normalize(glm::vec3(std::cos(rightPercentAroundCircle * 2 * PI),
                                                    0.5,
                                                    std::sin(rightPercentAroundCircle * 2 * PI)));
        auto leftNormal = glm::normalize(glm::vec3(std::cos(leftPercentAroundCircle * 2 * PI),
                                                   0.5,
                                                   std::sin(leftPercentAroundCircle * 2 * PI)));
        auto peakNormal = glm::normalize(glm::cross(rightCoord - leftCoord, peak - leftCoord));

        // create the triangle
        surfaces.push_back(std::make_unique<SimpleTriangle>(SimpleTriangle(
            leftCoord, rightCoord, peak, verticalDivisions, leftNormal, rightNormal, peakNormal)));
    }

    // append all surfaces together
    for (auto &surface : surfaces) {
        std::vector<GLfloat> surfaceTris = surface->getVerticesAndNormals();
        m_vertexData.insert(m_vertexData.begin(), surfaceTris.begin(), surfaceTris.end());
    }

    /**
     * Initialize the OpenGLShapeProperties so that the shape is ready to be
     * drawn. Note that this does not actually draw the shape, that happens in
     * ShapesScene.cpp
     */
    initializeOpenGLShapeProperties();
}
