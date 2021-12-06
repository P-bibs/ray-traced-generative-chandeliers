#include "Filter.h"
#include "RGBA.h"

void separableConvolve(float *horizontalKernel,
                       float *verticalKernel,
                       int kernelRadius,
                       glm::vec3 *input,
                       int width,
                       int height,
                       glm::vec3 *output,
                       bool normalize) {

    // set up an intermediate buffer for the vertical pass
    std::vector<glm::vec3> bufVector(width * height);
    glm::vec3 *buf = bufVector.data();

    // vertical pass
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

            // keep track of sum of convolution values as well as sum of weights so we can normalize
            // at the end
            glm::vec3 sum = glm::vec3(0.0f, 0.0f, 0.0f);
            float weights_sum = 0.0;

            // loop over the kernel indices
            for (size_t kernelIndex = 0; kernelIndex < kernelRadius * 2 + 1; kernelIndex++) {
                size_t offset = kernelIndex - kernelRadius;

                // make sure the current pixel is in-bounds
                if (r + offset >= 0 && r + offset < height) {
                    // overlay this pixel of the kernel on the image to find the relevant pixel
                    size_t index = (r + offset) * width + c;

                    sum += input[index] * verticalKernel[kernelIndex];
                    weights_sum += verticalKernel[kernelIndex];
                }
            }
            // normalize results
            glm::vec3 result = normalize ? sum / weights_sum : sum;
            buf[centerIndex] = result;
        }
    }

    // same as above but for horizontal pass
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t centerIndex = r * width + c;

            glm::vec3 sum = glm::vec3(0.0f);
            float weights_sum = 0.0;

            for (size_t kernelIndex = 0; kernelIndex < kernelRadius * 2 + 1; kernelIndex++) {
                size_t offset = kernelIndex - kernelRadius;

                if (c + offset >= 0 && c + offset < width) {
                    size_t index = r * width + (c + offset);

                    sum += buf[index] * horizontalKernel[kernelIndex];
                    weights_sum += horizontalKernel[kernelIndex];
                }
            }
            glm::vec3 result = normalize ? sum / weights_sum : sum;
            output[centerIndex] = result;
        }
    }
}

void edgeDetect(int width, int height, glm::vec3 *input, glm::vec3 *output) {
    // convert the image to gray scale
    toGrayScale(input, width, height);

    // set up intermediate buffers for the horizontal and vertical passes
    std::vector<glm::vec3> xGradVector(width * height);
    glm::vec3 *xGrad = xGradVector.data();
    std::vector<glm::vec3> yGradVector(width * height);
    glm::vec3 *yGrad = yGradVector.data();

    // sobel filters
    float sobelXHorizontal[3] = {-1, 0, 1};
    float sobelXVertical[3] = {1, 2, 1};
    float sobelYHorizontal[3] = {1, 2, 1};
    float sobelYVertical[3] = {1, 0, -1};

    // perform edge detection
    separableConvolve(sobelXHorizontal, sobelXVertical, 1, input, width, height, xGrad, false);
    separableConvolve(sobelYHorizontal, sobelYVertical, 1, input, width, height, yGrad, false);

    // combine horizontal and vertical passes ,taking into account sensitivity
    for (size_t i = 0; i < height * width; i++) {
        float mag = settings.edgeDetectSensitivity *
                    sqrt(xGrad[i].r * xGrad[i].r + yGrad[i].r * yGrad[i].r);
        output[i].r = mag;
        output[i].g = mag;
        output[i].b = mag;
    }
}

void gaussianBlur(int width, int height, glm::vec3 *input, glm::vec3 *output, int radius) {
    // sigma should be r / 3
    float kernel[radius * 2 + 1];
    float sigma = radius / 3.0f;

    // fill kernel with a 1D gaussian distribution and normalize
    float sum = 0;
    for (int i = -radius; i <= radius; i++) {
        float output = gaussian(i, 0, sigma);
        sum += output;
        kernel[i + radius] = output;
    }
    for (int i = -radius; i <= radius; i++) {
        kernel[i + radius] /= sum;
    }

    // perform convolution
    separableConvolve(kernel, kernel, radius, input, width, height, output, true);
}

// calculate gaussian with mean m and sigma s at point x
float gaussian(float x, float m, float s) {
    float a = 1 / (sqrt(2 * M_PI) * s);
    float b = std::exp(-(x * x / (2 * s * s)));
    return a * b;
}

void toGrayScale(glm::vec3 *image, int width, int height) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            size_t index = r * width + c;
            float gray = 0.299 * image[index].r + 0.587 * image[index].g + 0.114 * image[index].b;

            image[index] = glm::vec3(gray, gray, gray);
        }
    }
}

void scaleImageX(glm::vec3 *input,
                 int width,
                 int height,
                 float a,
                 glm::vec3 *output,
                 int outputWidth,
                 int outputHeight) {

    for (int row = 0; row < outputHeight; row++) {
        for (int col = 0; col < outputWidth; col++) {

            // keep track of value and weights so we can normalize
            glm::vec3 sum = glm::vec3(0.0, 0.0, 0.0);
            float weights_sum = 0.0;

            float center = (col / a) + (1.0 - a) / (2.0 * a);
            float support = (a > 1.0) ? 1.0 : 1.0 / a;

            int left = ceil(center - support);
            int right = floor(center + support);

            for (int i = left; i <= right; i++) {
                int columnInInput = i;

                // bounds check
                if (columnInInput < 0 || columnInInput >= width) {
                    continue;
                }

                int index = row * width + columnInInput;

                sum += triangle(i - center, a) * input[index];
                weights_sum += triangle(i - center, a);
            }

            // normalize results
            glm::vec3 result = sum / weights_sum;
            output[row * outputWidth + col] = result;
        }
    }
}

void scaleImageY(glm::vec3 *input,
                 int width,
                 int height,
                 float a,
                 glm::vec3 *output,
                 int outputWidth,
                 int outputHeight) {

    // same as above code for scaleImageX

    for (int row = 0; row < outputHeight; row++) {
        for (int col = 0; col < outputWidth; col++) {

            glm::vec3 sum = glm::vec3(0.0, 0.0, 0.0);
            float weights_sum = 0.0;

            float center = (row / a) + (1.0 - a) / (2.0 * a);
            float support = (a > 1.0) ? 1.0 : 1.0 / a;

            int top = ceil(center - support);
            int bottom = floor(center + support);

            for (int i = top; i <= bottom; i++) {
                int rowInInput = i;
                if (rowInInput < 0 || rowInInput >= height) {
                    continue;
                }

                int index = rowInInput * width + col;

                sum += triangle(i - center, a) * input[index];
                weights_sum += triangle(i - center, a);
            }

            glm::vec3 result = sum / weights_sum;

            output[row * outputWidth + col] = result;
        }
    }
}

// calculate triangle filter for scale factor `a` at point `x`
float triangle(float x, float a) {
    double radius;
    if (a < 1) {
        radius = 1.0 / a;
    } else {
        radius = 1.0;
    }

    if ((x < -radius) || (x > radius)) {
        return 0.0;
    } else {
        return (1.0 - fabs(x) / radius) / radius;
    }
}
