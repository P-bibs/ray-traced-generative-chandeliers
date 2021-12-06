#ifndef IMPLICITPLANE_H
#define IMPLICITPLANE_H

#include "Intersectable.h"

class ImplicitPlane : public Intersectable {
  public:
    ImplicitPlane(glm::vec3 point, glm::vec3 normal);

    IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) override;
    glm::vec4 getNormal();

  private:
    glm::vec4 m_point;
    glm::vec4 m_normal;
};

#endif // IMPLICITPLANE_H
