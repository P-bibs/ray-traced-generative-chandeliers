#ifndef FILTERUTILS_H
#define FILTERUTILS_H

#include "Canvas2D.h"
#include <math.h>
#include "RGBA.h"

inline unsigned char REALtoCOLOR(float f) {
    int i = static_cast<int>((f * 255.0 + 0.5));
    return (i < 0) ? 0 : (i > 255) ? 255 : i;
}

inline float backmap(int i, float scale) {
    return i/scale + (1 - scale)/(2*scale);
}

inline float gaussian(float x, float sigma) {
    return 1.0/(sqrt(2 * M_PI) * sigma) * pow(M_E,(- x * x / (2*sigma*sigma)));
}

inline float cone(float x, float y, float radiusX, float radiusY) {
    return std::max(1 - sqrt(x * x / (radiusX * radiusX) + y * y / (radiusY * radiusY)),0.0f);
}

inline float toRadians(float degrees) {
    return degrees * M_PI/180.0;
}

inline int getIndex(int r, int c, int height, int width)
{
    return r * width + c;
}

inline bool inImage(int r, int c, int height, int width)
{
    return r >= 0 && r < height && c >= 0 && c < width;
}

inline bool inImage(float r, float c, int height, int width)
{
    return r >= 0 && r < height && c >= 0 && c < width;
}


inline unsigned char RGBAToGray(const RGBA &pixel)
{
    return (0.299f * (float)pixel.r) +
           (0.587f * (float)pixel.g) +
           (0.114f * (float)pixel.b) + 0.5;
}



class FilterUtils
{
public:
    static RGBAfloat convolvePoint(RGBAfloat* data, int height, int width, float* kernel, int kWidth, int kHeight, int r, int c);
    static void convolve(RGBAfloat* data, int height, int width, float* kernel, int kWidth, int kHeight);
    static void blur(RGBAfloat* data, int width, int height, int radius);
};

#endif // FILTERUTILS_H
