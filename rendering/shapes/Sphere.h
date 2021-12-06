#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGLShape.h"

class Sphere : public OpenGLShape {
  public:
    Sphere(int param1, int param2);

  private:
    glm::vec3 sphericalToCartesian(glm::vec3 spherical);
};

#endif // SPHERE_H
