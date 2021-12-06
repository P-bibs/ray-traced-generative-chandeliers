#ifndef IMPLICITCUBE_H
#define IMPLICITCUBE_H

#include "ImplicitPlane.h"
#include "Intersectable.h"
#include <algorithm>
#include <optional>
#include <vector>
class ImplicitCube : public Intersectable {
  public:
    ImplicitCube();

    IntersectResult intersectRay(glm::vec4 P, glm::vec4 d) override;

  private:
    std::unique_ptr<ImplicitPlane> m_topPlane;
    std::unique_ptr<ImplicitPlane> m_bottomPlane;
    std::unique_ptr<ImplicitPlane> m_leftPlane;
    std::unique_ptr<ImplicitPlane> m_rightPlane;
    std::unique_ptr<ImplicitPlane> m_frontPlane;
    std::unique_ptr<ImplicitPlane> m_backPlane;
};

#endif // IMPLICITCUBE_H
