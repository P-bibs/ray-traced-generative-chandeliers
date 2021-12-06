#ifndef ISOCELESTRIANGLE_H
#define ISOCELESTRIANGLE_H

#include "Surface.h"

class IsocelesTriangle : public Surface {
  public:
    IsocelesTriangle(glm::vec3 bottomLeft,
                     glm::vec3 bottomRight,
                     glm::vec3 top,
                     int verticalSubdivisions,
                     int horizontalSubdivisions);
    std::vector<GLfloat> getVerticesAndNormals() override;

  private:
    glm::vec3 m_bottomLeft;
    glm::vec3 m_bottomRight;
    glm::vec3 m_top;
    int m_horizontalSubdivisions;
    int m_verticalSubdivisions;
};

#endif // ISOCELESTRIANGLE_H
