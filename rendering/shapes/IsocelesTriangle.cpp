#include "IsocelesTriangle.h"
#include "iostream"

IsocelesTriangle::IsocelesTriangle(glm::vec3 bottomLeft,
                                   glm::vec3 bottomRight,
                                   glm::vec3 top,
                                   int verticalSubdivisions,
                                   int horizontalSubdivisions)
    : m_bottomLeft(bottomLeft)
    , m_bottomRight(bottomRight)
    , m_top(top)
    , m_horizontalSubdivisions(horizontalSubdivisions)
    , m_verticalSubdivisions(verticalSubdivisions) {}

std::vector<GLfloat> IsocelesTriangle::getVerticesAndNormals() {
    auto leftLegVector = m_top - m_bottomLeft;
    auto rightLegVector = m_top - m_bottomRight;

    auto vertices = std::vector<std::vector<glm::vec3>>();

    // Start by finding the vertices where the triangles will be made
    for (int i = 0; i < m_verticalSubdivisions + 1; i++) {
        vertices.push_back(std::vector<glm::vec3>());

        // Move the baseline of the triangle up and subdivide horizontally into
        // further triangles
        glm::vec3 baselineLeftCoord =
            m_bottomLeft + (leftLegVector * (static_cast<float>(i) / m_verticalSubdivisions));
        glm::vec3 baselineRightCoord =
            m_bottomRight + (rightLegVector * (static_cast<float>(i) / m_verticalSubdivisions));
        glm::vec3 baselineVectorLeftToRight = baselineRightCoord - baselineLeftCoord;

        int subdivisionsInCurrentRow = m_horizontalSubdivisions - i;
        for (int j = 0; j < subdivisionsInCurrentRow + 1; j++) {
            if (i == m_verticalSubdivisions && j == subdivisionsInCurrentRow) {
                vertices[i].push_back(m_top);
            } else {
                vertices[i].push_back(baselineLeftCoord +
                                      (baselineVectorLeftToRight *
                                       (static_cast<float>(j) / subdivisionsInCurrentRow)));
            }
        }
    }

    // Calculate one normal for all triangles
    glm::vec3 normal =
        glm::normalize(glm::cross(m_bottomRight - m_bottomLeft, m_top - m_bottomLeft));

    auto verticesAndNormals = std::vector<glm::vec3>();

    // Create triangles based on previously found vertices
    for (int i = 0; i < vertices.size(); i++) {
        for (int j = 0; j < vertices[i].size() - 1; j++) {
            {
                glm::vec3 bottomLeft = vertices[i][j];
                glm::vec3 bottomRight = vertices[i][j + 1];
                glm::vec3 top = vertices[i + 1][j];

                verticesAndNormals.push_back(bottomLeft);
                verticesAndNormals.push_back(normal);

                verticesAndNormals.push_back(bottomRight);
                verticesAndNormals.push_back(normal);

                verticesAndNormals.push_back(top);
                verticesAndNormals.push_back(normal);
            }

            if (i != 0) {
                glm::vec3 bottomLeft = vertices[i][j];
                glm::vec3 bottomRight = vertices[i][j + 1];
                glm::vec3 top = vertices[i - 1][j + 1];

                verticesAndNormals.push_back(top);
                verticesAndNormals.push_back(normal);

                verticesAndNormals.push_back(bottomRight);
                verticesAndNormals.push_back(normal);

                verticesAndNormals.push_back(bottomLeft);
                verticesAndNormals.push_back(normal);
            }
        }
    }

    // turn vertex array into float array
    auto out = std::vector<GLfloat>();
    for (auto &vec : verticesAndNormals) {
        out.push_back(vec.x);
        out.push_back(vec.y);
        out.push_back(vec.z);
    }

    return out;
}
