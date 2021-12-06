#ifndef IMPLICITCYLINDER_H
#define IMPLICITCYLINDER_H

#include "ImplicitPlane.h"
#include "ImplicitShaft.h"
#include "Intersectable.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

class ImplicitCylinder : public Intersectable {
  public:
    ImplicitCylinder();
    IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) override;

  private:
    std::unique_ptr<ImplicitShaft> m_shaft;
    std::unique_ptr<ImplicitPlane> m_topPlane;
    std::unique_ptr<ImplicitPlane> m_bottomPlane;
};

#endif // IMPLICITCYLINDER_H
