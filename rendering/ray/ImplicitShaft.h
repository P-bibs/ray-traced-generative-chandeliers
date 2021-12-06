#ifndef IMPLICITSHAFT_H
#define IMPLICITSHAFT_H

#include "Intersectable.h"
#include <optional>
#include <vector>

class ImplicitShaft : public Intersectable {
  public:
    ImplicitShaft();

    IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) override;
};

#endif // IMPLICITSHAFT_H
