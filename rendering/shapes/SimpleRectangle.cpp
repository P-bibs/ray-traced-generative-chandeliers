#include "SimpleRectangle.h"
#include "iostream"

SimpleRectangle::SimpleRectangle(glm::vec3 bottomLeft,
                                 glm::vec3 bottomRight,
                                 glm::vec3 topRight,
                                 glm::vec3 topLeft,
                                 int subdivisions)
    : m_bottomLeft(bottomLeft)
    , m_bottomRight(bottomRight)
    , m_topRight(topRight)
    , m_topLeft(topLeft)
    , m_subdivisions(subdivisions)
    , m_leftNormal(
          glm::normalize(glm::cross(m_bottomRight - m_bottomLeft, m_topLeft - m_bottomLeft)))
    , m_rightNormal(
          glm::normalize(glm::cross(m_bottomRight - m_bottomLeft, m_topLeft - m_bottomLeft))) {}

SimpleRectangle::SimpleRectangle(glm::vec3 bottomLeft,
                                 glm::vec3 bottomRight,
                                 glm::vec3 topRight,
                                 glm::vec3 topLeft,
                                 int subdivisions,
                                 glm::vec3 leftNormal,
                                 glm::vec3 rightNormal)
    : m_bottomLeft(bottomLeft)
    , m_bottomRight(bottomRight)
    , m_topRight(topRight)
    , m_topLeft(topLeft)
    , m_subdivisions(subdivisions)
    , m_leftNormal(leftNormal)
    , m_rightNormal(rightNormal) {}

std::vector<GLfloat> SimpleRectangle::getVerticesAndNormals() {
    auto leftSideVector = m_topLeft - m_bottomLeft;
    auto rightSideVector = m_topRight - m_bottomRight;

    auto leftVertices = std::vector<glm::vec3>();
    auto rightVertices = std::vector<glm::vec3>();

    // calculate vertices to divide the rectangle up into smaller rectangles made of 2 triangles
    for (int i = 0; i < m_subdivisions + 1; i++) {
        leftVertices.push_back(m_bottomLeft +
                               (leftSideVector * (static_cast<float>(i) / m_subdivisions)));
        rightVertices.push_back(m_bottomRight +
                                (rightSideVector * (static_cast<float>(i) / m_subdivisions)));
    }

    // Turn vertices into triangles with normals
    auto trisAndNormals = std::vector<glm::vec3>();
    for (int i = 0; i < m_subdivisions; i++) {
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

    // Turn vertex array into float array
    auto out = std::vector<GLfloat>();
    for (auto &vec : trisAndNormals) {
        out.push_back(vec.x);
        out.push_back(vec.y);
        out.push_back(vec.z);
    }

    return out;
}