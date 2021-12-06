#ifndef SQUARE_H
#define SQUARE_H

#include "Surface.h"
#include "optional"

class Square : public Surface {
  public:
    Square(glm::vec3 bottomLeft,
           glm::vec3 bottomRight,
           glm::vec3 topRight,
           glm::vec3 topLeft,
           int subdivisions);
    std::vector<GLfloat> getVerticesAndNormals() override;

  private:
    int m_subdivisions;
    glm::vec3 m_bottomLeft;
    glm::vec3 m_bottomRight;
    glm::vec3 m_topRight;
    glm::vec3 m_topLeft;
};

#endif // SQUARE_H
