#ifndef IMPLICITCONE_H
#define IMPLICITCONE_H

#include "ImplicitPlane.h"
#include "Intersectable.h"
#include <optional>
#include <vector>
class ImplicitCone : public Intersectable {
  public:
    ImplicitCone();

    IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) override;

  private:
    std::unique_ptr<ImplicitPlane> m_bottomFace;
};

#endif // IMPLICITCONE_H
