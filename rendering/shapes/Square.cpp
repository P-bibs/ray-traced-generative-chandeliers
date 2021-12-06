#include "Square.h"
#include "IsocelesTriangle.h"
#include "vector"

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <memory>

Square::Square(glm::vec3 bottomLeft,
               glm::vec3 bottomRight,
               glm::vec3 topRight,
               glm::vec3 topLeft,
               int subdivisions)
    : m_subdivisions(subdivisions)
    , m_bottomLeft(bottomLeft)
    , m_bottomRight(bottomRight)
    , m_topRight(topRight)
    , m_topLeft(topLeft) {}

std::vector<GLfloat> Square::getVerticesAndNormals() {
    // Create a square by combining two triangles

    auto tri1 = std::make_unique<IsocelesTriangle>(
        IsocelesTriangle(m_bottomLeft, m_topRight, m_topLeft, m_subdivisions, m_subdivisions));
    auto tri2 = std::make_unique<IsocelesTriangle>(
        IsocelesTriangle(m_bottomLeft, m_bottomRight, m_topRight, m_subdivisions, m_subdivisions));

    auto out = std::vector<GLfloat>();

    auto tri1Verts = tri1->getVerticesAndNormals();
    auto tri2Verts = tri2->getVerticesAndNormals();

    out.insert(out.end(), tri1Verts.begin(), tri1Verts.end());
    out.insert(out.end(), tri2Verts.begin(), tri2Verts.end());

    return out;
}
