#ifndef FILTER_H
#define FILTER_H

#include "RGBA.h"
#include <Settings.h>
#include <algorithm>
#include <vector>

// public

// scale an image horizontally by scale factor `a` and put the output in `output` (pixel values 0-1)
void scaleImageX(glm::vec3 *input,
                 int width,
                 int height,
                 float a,
                 glm::vec3 *output,
                 int outputWidth,
                 int outputHeight);
// scale an image vertically by scale factor `a` and put the output in `output` (pixel values 0-1)
void scaleImageY(glm::vec3 *input,
                 int width,
                 int height,
                 float a,
                 glm::vec3 *output,
                 int outputWidth,
                 int outputHeight);
// perform edge detection on `input` and put the output in `output` (pixel values 0-1)
void edgeDetect(int width, int height, glm::vec3 *input, glm::vec3 *output);
// blur `input` with radius `radius` and put the output in `output` (pixel values 0-1)
void gaussianBlur(int width, int height, glm::vec3 *input, glm::vec3 *output, int radius);

// private

// perform convolution with a separable kernel (pixel values 0-1)
void separableConvolve(float *horizontalKernel,
                       float *verticalKernel,
                       int kernelRadius,
                       glm::vec3 *image,
                       int width,
                       int height,
                       glm::vec3 *output,
                       bool normalize);
// convert `image` to gray scale in-place
void toGrayScale(glm::vec3 *image, int width, int height);
// calculate gaussian distribution with mean `m` and standard deviation `s`
float gaussian(float x, float m, float s);
// calculate triangle filter with scale factor `a` at point `x`
float triangle(float x, float a);

#endif // FILTER_H
