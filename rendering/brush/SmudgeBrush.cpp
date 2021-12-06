/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"

#include "Canvas2D.h"

#include <math.h>
#include <iostream>

SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius),
    m_smudgeBuffer(std::vector<RGBA>())
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!

    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //

    // Initialize mask with distance squared values
    fillMaskWithDistanceSquared();

    // Everytime we make the mask, we also want to size the smudge buffer
    m_smudgeBuffer.clear();
    m_smudgeBuffer.reserve(m_mask.size());
    for (int i = 0; i < m_mask.size(); i++) {
        // Use alpha=0 default so if we paint with an initialized pixel
        // the canvas doesn't change color
        m_smudgeBuffer.push_back(RGBA(0,0,0,0));
    }

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

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //

    // Below logic is copied from Brush::brushDragged but with a simplified
    // loop body
    RGBA *pix = canvas->data();

    int R = m_radius;
    int width = canvas->width();
    int height = canvas->height();
    int maskWidth = R * 2 + 1;

    int rowStartUnsafe = y - R;
    int rowStart = std::max(0, rowStartUnsafe);
    int rowEnd = std::min(height, y + R + 1);
    int colStartUnsafe = x - R;
    int colStart = std::max(0, colStartUnsafe);
    int colEnd = std::min(width, x + R + 1);

    for (int rowCounter = rowStart; rowCounter < rowEnd; rowCounter++) {
        for (int colCounter = colStart; colCounter < colEnd; colCounter++) {
            m_smudgeBuffer[(rowCounter - rowStartUnsafe) * maskWidth + (colCounter - colStartUnsafe)] =
                    pix[rowCounter * canvas->width() + colCounter];
        }
    }

}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.


    // The below is copied from Brush::brushDragged but with some slight modifications
    // to the call to the color blend function
    RGBA *pix = canvas->data();

    int R = m_radius;
    int width = canvas->width();
    int height = canvas->height();
    int maskWidth = R * 2 + 1;

    int rowStartUnsafe = mouseY - R;
    int rowStart = std::max(0, rowStartUnsafe);
    int rowEnd = std::min(height, mouseY + R + 1);
    int colStartUnsafe = mouseX - R;
    int colStart = std::max(0, colStartUnsafe);
    int colEnd = std::min(width, mouseX + R + 1);

    for (int rowCounter = rowStart; rowCounter < rowEnd; rowCounter++) {
        for (int colCounter = colStart; colCounter < colEnd; colCounter++) {
            float maskValue = getMaskValue(colCounter - colStartUnsafe, rowCounter - rowStartUnsafe);
            RGBA smudgeColor = m_smudgeBuffer[(rowCounter - rowStartUnsafe) * maskWidth + (colCounter - colStartUnsafe)];
            RGBA canvasColor = pix[rowCounter * canvas->width() + colCounter];

            // Calculate blended colors.
            // Use alpha from smudge color so that we have alpha=0 if some pixels in the smudge buffer
            // came from off the screen.
            int red = blendColors(maskValue, smudgeColor.a,   smudgeColor.r, canvasColor.r)   * 255;
            int green = blendColors(maskValue, smudgeColor.a, smudgeColor.g, canvasColor.g)   * 255;
            int blue = blendColors(maskValue, smudgeColor.a,  smudgeColor.b, canvasColor.b)   * 255;

            // Write blended colors to canvas
            pix[rowCounter * canvas->width() + colCounter] = RGBA(red, green, blue, 255);
        }
    }

    canvas->update();

    // now pick up paint again...
    pickUpPaint(mouseX, mouseY, canvas);
}


