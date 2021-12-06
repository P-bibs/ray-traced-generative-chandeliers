#ifndef SURFACE_H
#define SURFACE_H

#include "GL/glew.h"
#include <glm/glm.hpp>

#include <vector>

class Surface {
  public:
    Surface();
    virtual std::vector<GLfloat> getVerticesAndNormals() = 0;
};

#endif // SURFACE_H
