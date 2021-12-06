#include "Cylinder.h"
#include "Disc.h"
#include "SimpleRectangle.h"
#include "SimpleTriangle.h"

Cylinder::Cylinder() {}

Cylinder::Cylinder(int param1, int param2) {
    int verticalDivisions = std::max(1, param1);
    int radialDivisions = std::max(3, param2);

    float PI = M_PI;

    auto surfaces = std::vector<std::unique_ptr<Surface>>();

    // Create the top disc of the cylinder
    surfaces.push_back(std::make_unique<Disc>(
        Disc(glm::vec3(0, 0.5, 0), verticalDivisions, radialDivisions, true)));

    // Create the base disc of the cylinder
    surfaces.push_back(std::make_unique<Disc>(
        Disc(glm::vec3(0, -0.5, 0), verticalDivisions, radialDivisions, false)));

    // Create the rectangles that make up the barrel of the cylinder
    for (int i = 0; i < radialDivisions; i++) {
        // Divide the circular base up into a series of intervals
        auto percentAroundCircle1 = static_cast<float>(i) / radialDivisions;
        auto percentAroundCircle2 = static_cast<float>(i + 1) / radialDivisions;

        // Calculate the cartesian coordinates of the four rectangular points
        auto bottomRight = glm::vec3(0.5 * std::cos(percentAroundCircle1 * 2 * PI),
                                     -0.5,
                                     0.5 * std::sin(percentAroundCircle1 * 2 * PI));
        auto bottomLeft = glm::vec3(0.5 * std::cos(percentAroundCircle2 * 2 * PI),
                                    -0.5,
                                    0.5 * std::sin(percentAroundCircle2 * 2 * PI));
        auto topRight = bottomRight + glm::vec3(0, 1, 0);
        auto topLeft = bottomLeft + glm::vec3(0, 1, 0);

        // calculate normals
        auto rightNormal = glm::normalize(glm::vec3(
            std::cos(percentAroundCircle1 * 2 * PI), 0, std::sin(percentAroundCircle1 * 2 * PI)));
        auto leftNormal = glm::normalize(glm::vec3(
            std::cos(percentAroundCircle2 * 2 * PI), 0, std::sin(percentAroundCircle2 * 2 * PI)));

        surfaces.push_back(std::make_unique<SimpleRectangle>(SimpleRectangle(bottomLeft,
                                                                             bottomRight,
                                                                             topRight,
                                                                             topLeft,
                                                                             verticalDivisions,
                                                                             leftNormal,
                                                                             rightNormal)));
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
