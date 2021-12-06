#ifndef CUBE_H
#define CUBE_H

#include "OpenGLShape.h"

class Cube : public OpenGLShape {
  public:
    Cube();
    Cube(int param1, int param2);

  private:
    int m_param1;
    int m_param2;
};

#endif // CUBE_H
