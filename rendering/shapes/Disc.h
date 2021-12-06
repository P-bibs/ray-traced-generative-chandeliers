#ifndef DISC_H
#define DISC_H

#include "Surface.h"

class Disc : public Surface {
  public:
    Disc(glm::vec3 center, int sideCount, int verticalDivisions, bool facingUp);
    std::vector<GLfloat> getVerticesAndNormals() override;

  private:
    glm::vec3 m_center;
    int m_verticalDivisions;
    int m_radialDivisions;
    bool m_facingUp;
};

#endif // DISC_H
