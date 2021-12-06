#include "SimpleTriangle.h"
#include "iostream"
#include "vector"

SimpleTriangle::SimpleTriangle(glm::vec3 bottomLeft,
                               glm::vec3 bottomRight,
                               glm::vec3 top,
                               int subdivisions)
    : m_bottomLeft(bottomLeft)
    , m_bottomRight(bottomRight)
    , m_top(top)
    , m_subdivisions(subdivisions)
    , m_leftNormal(glm::normalize(glm::cross(bottomRight - bottomLeft, top - bottomLeft)))
    , m_rightNormal(glm::normalize(glm::cross(bottomRight - bottomLeft, top - bottomLeft)))
    , m_topNormal(glm::normalize(glm::cross(bottomRight - bottomLeft, top - bottomLeft))) {}

SimpleTriangle::SimpleTriangle(glm::vec3 bottomLeft,
                               glm::vec3 bottomRight,
                               glm::vec3 top,
                               int subdivisions,
                               glm::vec3 leftNormal,
                               glm::vec3 rightNormal,
                               glm::vec3 topNormal)
    : m_bottomLeft(bottomLeft)
    , m_bottomRight(bottomRight)
    , m_top(top)
    , m_subdivisions(subdivisions)
    , m_leftNormal(glm::vec3(leftNormal))
    , m_rightNormal(glm::vec3(rightNormal))
    , m_topNormal(glm::vec3(topNormal)) {}

std::vector<GLfloat> SimpleTriangle::getVerticesAndNormals() {
    auto leftLegVector = m_top - m_bottomLeft;
    auto rightLegVector = m_top - m_bottomRight;

    auto leftVertices = std::vector<glm::vec3>();
    auto rightVertices = std::vector<glm::vec3>();

    // calculate vertices to divide the triangle up into smaller rectangles made of 2 triangles
    for (int i = 0; i < m_subdivisions; i++) {
        leftVertices.push_back(m_bottomLeft +
                               (leftLegVector * (static_cast<float>(i) / m_subdivisions)));
        rightVertices.push_back(m_bottomRight +
                                (rightLegVector * (static_cast<float>(i) / m_subdivisions)));
    }

    // Turn vertices into triangles with normals
    auto trisAndNormals = std::vector<glm::vec3>();
    for (int i = 0; i < m_subdivisions - 1; i++) {
        glm::vec3 bottomLeft = leftVertices[i];
        glm::vec3 bottomRight = rightVertices[i];
        glm::vec3 topLeft = leftVertices[i + 1];
        glm::vec3 topRight = rightVertices[i + 1];

        trisAndNormals.push_back(bottomLeft);
        trisAndNormals.push_back(m_leftNormal);
        trisAndNormals.push_back(bottomRight);
        trisAndNormals.push_back(m_rightNormal);
        trisAndNormals.push_back(topRight);
        trisAndNormals.push_back(m_rightNormal);

        trisAndNormals.push_back(bottomLeft);
        trisAndNormals.push_back(m_leftNormal);
        trisAndNormals.push_back(topRight);
        trisAndNormals.push_back(m_rightNormal);
        trisAndNormals.push_back(topLeft);
        trisAndNormals.push_back(m_leftNormal);
    }

    // Add top triangle
    trisAndNormals.push_back(leftVertices.back());
    trisAndNormals.push_back(m_leftNormal);
    trisAndNormals.push_back(rightVertices.back());
    trisAndNormals.push_back(m_rightNormal);
    trisAndNormals.push_back(m_top);
    trisAndNormals.push_back(m_topNormal);

    // Turn vertices into triangles with normals
    auto out = std::vector<GLfloat>();
    for (auto &vec : trisAndNormals) {
        out.push_back(vec.x);
        out.push_back(vec.y);
        out.push_back(vec.z);
    }

    return out;
}
