#include "Cube.h"
#include "IsocelesTriangle.h"
#include "Square.h"
#include "vector"

Cube::Cube() {}

Cube::Cube(int param1, int param2)
    : m_param1(param1)
    , m_param2(param2) {

    auto surfaces = std::vector<std::unique_ptr<Surface>>();

    // Front face
    surfaces.push_back(std::make_unique<Square>(Square(glm::vec3(-0.5, -0.5, 0.5),
                                                       glm::vec3(0.5, -0.5, 0.5),
                                                       glm::vec3(0.5, 0.5, 0.5),
                                                       glm::vec3(-0.5, 0.5, 0.5),
                                                       m_param1)));
    // Bottom face
    surfaces.push_back(std::make_unique<Square>(Square(glm::vec3(-0.5, -0.5, -0.5),
                                                       glm::vec3(0.5, -0.5, -0.5),
                                                       glm::vec3(0.5, -0.5, 0.5),
                                                       glm::vec3(-0.5, -0.5, 0.5),
                                                       m_param1)));
    // Top face
    surfaces.push_back(std::make_unique<Square>(Square(glm::vec3(0.5, 0.5, -0.5),
                                                       glm::vec3(-0.5, 0.5, -0.5),
                                                       glm::vec3(-0.5, 0.5, 0.5),
                                                       glm::vec3(0.5, 0.5, 0.5),
                                                       m_param1)));
    // Left face
    surfaces.push_back(std::make_unique<Square>(Square(glm::vec3(-0.5, -0.5, -0.5),
                                                       glm::vec3(-0.5, -0.5, 0.5),
                                                       glm::vec3(-0.5, 0.5, 0.5),
                                                       glm::vec3(-0.5, 0.5, -0.5),
                                                       m_param1)));
    // Right face
    surfaces.push_back(std::make_unique<Square>(Square(glm::vec3(0.5, -0.5, 0.5),
                                                       glm::vec3(0.5, -0.5, -0.5),
                                                       glm::vec3(0.5, 0.5, -0.5),
                                                       glm::vec3(0.5, 0.5, 0.5),
                                                       m_param1)));
    // Back face
    surfaces.push_back(std::make_unique<Square>(Square(glm::vec3(0.5, -0.5, -0.5),
                                                       glm::vec3(-0.5, -0.5, -0.5),
                                                       glm::vec3(-0.5, 0.5, -0.5),
                                                       glm::vec3(0.5, 0.5, -0.5),
                                                       m_param1)));

    // append surfaces together
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
