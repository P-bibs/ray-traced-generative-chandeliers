#ifndef SIMPLERECTANGLE_H
#define SIMPLERECTANGLE_H

#include "Surface.h"

class SimpleRectangle : public Surface {
  public:
    SimpleRectangle(glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    glm::vec3 topRight,
                    glm::vec3 topLeft,
                    int subdivisions);
    SimpleRectangle(glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    glm::vec3 topRight,
                    glm::vec3 topLeft,
                    int subdivisions,
                    glm::vec3 leftNormal,
                    glm::vec3 rightNormal);

    std::vector<GLfloat> getVerticesAndNormals() override;

  private:
    glm::vec3 m_bottomLeft;
    glm::vec3 m_bottomRight;
    glm::vec3 m_topRight;
    glm::vec3 m_topLeft;
    int m_subdivisions;
    glm::vec3 m_leftNormal;
    glm::vec3 m_rightNormal;
};

#endif // SIMPLERECTANGLE_H
