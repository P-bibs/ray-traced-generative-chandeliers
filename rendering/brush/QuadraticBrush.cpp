/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // Initialize mask with distance squared values
    fillMaskWithDistanceSquared();

    int radiusSquared = m_radius * m_radius;
    
    // constants for formula f(d) = Ad^2 + Bd + C from assignment handout
    float A = 1.0 / (m_radius * m_radius);
    float B = -(2.0 / m_radius);
    float C = 1.0;

    for (int i = 0; i < m_mask.size(); i++) {
        float distanceSquared = m_mask[i];

        // If this pixel is in range, then use the quadratic function we
        // calculated. Otherwise, set the mask to 0
        float maskValue;
        if (distanceSquared > radiusSquared) {
            maskValue = 0;
        } else {
            maskValue = std::max(0.0f, A * distanceSquared + B * (std::sqrt(distanceSquared)) + C);
        }

        // Write value to mask
        m_mask[i] =  maskValue;
    }
}


