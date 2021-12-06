#ifndef SIMPLETRIANGLE_H
#define SIMPLETRIANGLE_H

#include "Surface.h"

class SimpleTriangle : public Surface {
  public:
    SimpleTriangle(glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 top, int subdivisions);
    SimpleTriangle(glm::vec3 bottomLeft,
                   glm::vec3 bottomRight,
                   glm::vec3 top,
                   int subdivisions,
                   glm::vec3 leftNormal,
                   glm::vec3 rightNormal,
                   glm::vec3 topNormal);

    std::vector<GLfloat> getVerticesAndNormals() override;

  private:
    glm::vec3 m_bottomLeft;
    glm::vec3 m_bottomRight;
    glm::vec3 m_top;
    int m_subdivisions;
    glm::vec3 m_leftNormal;
    glm::vec3 m_rightNormal;
    glm::vec3 m_topNormal;
};

#endif // SIMPLETRIANGLE_H
