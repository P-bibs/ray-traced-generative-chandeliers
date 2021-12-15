#include "FilterUtils.h"

#include <math.h>
#include <iostream>

RGBAfloat FilterUtils::convolvePoint(RGBAfloat* data, int height, int width, float* kernel, int kWidth, int kHeight, int r, int c)
{
    RGBAfloat ret = RGBAfloat(0.0f,0.0f,0.0f,1.0f);
    float norm = 1.0;

    for(int kr = 0; kr < kHeight; kr++) {
        for(int kc = 0; kc < kWidth; kc++) {
            int r_off = (kr - kHeight/2);
            int c_off = (kc - kWidth/2);
            int k_off = r_off * width + c_off;
            int kernel_index = getIndex(kr,kc,kHeight,kWidth);

            if (!inImage(r + r_off,c + c_off,height,width)) {
                norm = norm - kernel[kernel_index];
                continue;
             }

             RGBAfloat pixel_val = data[getIndex(r,c,height,width) + k_off];
             float kernel_val = kernel[kernel_index];

             ret.r += pixel_val.r * kernel_val;
             ret.g += pixel_val.g * kernel_val;
             ret.b += pixel_val.b * kernel_val;
        }
    }

    return RGBAfloat(ret.r / norm,
                     ret.g / norm,
                     ret.b / norm,
                     1.0f);
}

void FilterUtils::convolve(RGBAfloat* data, int height, int width, float* kernel, int kWidth, int kHeight)
{
    RGBAfloat* result = new RGBAfloat[width * height];

    for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                int centerIndex = getIndex(r,c,height,width);
                RGBAfloat ret = FilterUtils::convolvePoint(data,height, width,kernel,kWidth,kHeight,r,c);
                result[centerIndex] = ret;
            }
    }

    memcpy(data,
           result,
           width *
           height *
           sizeof(RGBAfloat));
    delete[] result;
}

void FilterUtils::blur(RGBAfloat* data, int width, int height, int radius)
{
    int kSize = 2 * radius + 1;
    float kernel[kSize];
    float sigma = radius / 3.0;
    float normalization = 0.0;
    //populate the kernel with a 1D zero mean std radius/3 gaussian
    for(int i = 0; i < kSize; i++)
    {
        float x = i - radius;

        kernel[i] = gaussian(x,sigma);
        normalization += kernel[i];
    }

    //normalize the kernel (only really necessary when radius = 1)
    //but only happens once and is not a performance sink
    for(int i = 0; i < kSize; i++)
    {
        kernel[i] = kernel[i] / normalization;
    }



    //Implement blur using seperable gaussians
    FilterUtils::convolve(data, height, width, kernel, kSize, 1);
    FilterUtils::convolve(data, height, width, kernel, 1, kSize);
}
