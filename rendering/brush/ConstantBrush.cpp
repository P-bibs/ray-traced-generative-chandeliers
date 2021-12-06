/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"

#include <math.h>

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // initialized mask with distance squared values
    fillMaskWithDistanceSquared();
    
    int radiusSquared = m_radius * m_radius;

    for (int i = 0; i < m_mask.size(); i++) {
        float distanceSquared = m_mask[i];

        // If the pixel is too far away set the mask to 0, otherwise 1.
        float maskValue = distanceSquared < radiusSquared ? 1 : 0;

        // Write value to mask
        m_mask[i] =  maskValue;
    }
}


