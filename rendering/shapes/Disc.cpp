#include "Disc.h"
#include "shapes/SimpleTriangle.h"
#include <memory>
#include <vector>

Disc::Disc(glm::vec3 center, int verticalDivisions, int radialDivisions, bool facingUp)
    : m_center(center)
    , m_verticalDivisions(verticalDivisions)
    , m_radialDivisions(radialDivisions)
    , m_facingUp(facingUp) {}

std::vector<GLfloat> Disc::getVerticesAndNormals() {
    float PI = M_PI;

    auto surfaces = std::vector<std::unique_ptr<Surface>>();

    // create disc triangles
    for (int i = 0; i < m_radialDivisions; i++) {
        // divide 360 degrees into m_radialDivisions parts and find cartesian
        // coordinates for each point
        auto theta1 = static_cast<float>(i) / m_radialDivisions;
        auto theta2 = static_cast<float>(i + 1) / m_radialDivisions;
        auto coord1 =
            glm::vec3(0.5 * std::cos(theta1 * 2 * PI), m_center.y, 0.5 * std::sin(theta1 * 2 * PI));
        auto coord2 =
            glm::vec3(0.5 * std::cos(theta2 * 2 * PI), m_center.y, 0.5 * std::sin(theta2 * 2 * PI));

        // create a triangle for each division
        if (m_facingUp) {
            surfaces.push_back(std::make_unique<SimpleTriangle>(
                SimpleTriangle(coord2, coord1, m_center, m_verticalDivisions)));
        } else {
            surfaces.push_back(std::make_unique<SimpleTriangle>(
                SimpleTriangle(coord1, coord2, m_center, m_verticalDivisions)));
        }
    }

    auto out = std::vector<GLfloat>();

    // append triangles together
    for (auto &surface : surfaces) {
        std::vector<GLfloat> surfaceTris = surface->getVerticesAndNormals();
        out.insert(out.begin(), surfaceTris.begin(), surfaceTris.end());
    }

    return out;
}
