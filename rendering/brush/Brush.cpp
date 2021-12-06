/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"

#include <math.h>

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    m_color(color),
    m_mask(std::vector<float>()),
    m_radius(radius)
{
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}

float Brush::getMaskValue(int x, int y) {
    int maskWidth = 1 + this->m_radius * 2;
    return this->m_mask[maskWidth * y + x];
}

float Brush::blendColors(float maskValue, int alpha, int intensity, int canvasIntensity) {
    float convertedAlpha = alpha / 255.0;
    float convertedIntensity = intensity / 255.0;
    float convertedCanvasIntensity = canvasIntensity / 255.0;
    // From the algos homework:
    // M*a*B + (1-M*a)*C
    return maskValue * convertedAlpha * convertedIntensity +
            (1 - maskValue * convertedAlpha) * convertedCanvasIntensity;
}

void Brush::fillMaskWithDistanceSquared() {
    // Ensure the mask vector has enough room by filling with 0s.
    // TODO: the clear() may not be necessary
    m_mask.clear();
    int maxMaskSize = std::pow(m_radius * 2 + 1, 2);
    m_mask.reserve(maxMaskSize);
    for (int i = 0; i < maxMaskSize; i++) {
        m_mask.push_back(0);
    }

    int maskSideLength = m_radius * 2 + 1;
    int center = m_radius;

    // For every pixel in the mask, set its value to its distance squared
    // from the center of the mask. We'll use these values in the derived
    // brushes to calculate the actual mask
    for (int row = 0; row < maskSideLength; row++) {
        for (int col = 0; col < maskSideLength; col++) {
            int distanceSquared = std::pow(row - center, 2) + std::pow(col - center, 2);

            m_mask[row * maskSideLength + col] = distanceSquared;
        }
    }
}

void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    RGBA *pix = canvas->data();

    // shorten variable names
    int R = m_radius;
    int width = canvas->width();
    int height = canvas->height();

    // Set up the bounds for the loops
    int rowStartUnsafe = mouseY - R;
    int rowStart = std::max(0, rowStartUnsafe);
    int rowEnd = std::min(height, mouseY + R + 1);
    int colStartUnsafe = mouseX - R;
    int colStart = std::max(0, colStartUnsafe);
    int colEnd = std::min(width, mouseX + R + 1);

    for (int rowCounter = rowStart; rowCounter < rowEnd; rowCounter++) {
        for (int colCounter = colStart; colCounter < colEnd; colCounter++) {
            float maskValue = getMaskValue(colCounter - colStartUnsafe, rowCounter - rowStartUnsafe);
            int alpha = m_color.a;
            RGBA canvasValue = pix[rowCounter * canvas->width() + colCounter];

            // Calculate blended colors (scale by 255 to get values between 0 and 255)
            int red = blendColors(maskValue, alpha, m_color.r, canvasValue.r)   * 255;
            int green = blendColors(maskValue, alpha, m_color.g, canvasValue.g) * 255;
            int blue = blendColors(maskValue, alpha, m_color.b, canvasValue.b)  * 255;

            // Write blended colors to canvas
            pix[rowCounter * canvas->width() + colCounter] = RGBA(red, green, blue, 255);
        }
    }

    canvas->update();

}
