/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

#include <math.h>

LinearBrush::LinearBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
}

void LinearBrush::makeMask() {
    // initialize mask with distance squared values
    fillMaskWithDistanceSquared();
    
    int radiusSquared = m_radius * m_radius;

    for (int i = 0; i < m_mask.size(); i++) {
        float distanceSquared = m_mask[i];

        // Calculate mask value using linear equation
        float maskValue = std::max(0.0f, 1 - ((float) distanceSquared / (float) radiusSquared));

        // Write value to mask
        m_mask[i] =  maskValue;
    }
}


