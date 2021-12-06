#ifndef IMPLICITSPHERE_H
#define IMPLICITSPHERE_H

#include "Intersectable.h"

class ImplicitSphere : public Intersectable {
  public:
    ImplicitSphere();

    IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) override;
};

#endif // IMPLICITSPHERE_H
