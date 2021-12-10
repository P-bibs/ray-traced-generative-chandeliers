/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include "Canvas2D.h"
#include "RayScene.h"
#include "Settings.h"
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>

#include "Camera.h"
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "ray/RayTracer.h"
#include <QCoreApplication>
#include <QPainter>

#define K 0.1

Canvas2D::Canvas2D()
    : // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr)
    , m_brush(initializeBrush(settings.brushType))
    , m_currentRadius(settings.brushRadius)
    , m_currentColor(settings.brushColor)
    , m_currentBrushType(settings.brushType) {}

Canvas2D::~Canvas2D() {}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);
}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;

    // If the brush radius has changed, update the brush's radius
    if (m_currentRadius != settings.brushRadius) {
        m_currentRadius = settings.brushRadius;
        m_brush->setRadius(m_currentRadius);
    }

    // If the brush color has changed, update the brush's color
    if (m_currentColor != settings.brushColor) {
        m_currentColor = settings.brushColor;

        // If we wanted, we could check each individual color to see if it's changed,
        // but it's easier (and doesn't cost much extra) to just update them all.
        m_brush->setRed(settings.brushColor.r);
        m_brush->setGreen(settings.brushColor.g);
        m_brush->setBlue(settings.brushColor.b);
        m_brush->setAlpha(settings.brushColor.a);
    }

    // If the brush type has changed, create a new brush
    if (m_currentBrushType != settings.brushType) {
        m_currentBrushType = settings.brushType;

        m_brush = initializeBrush(m_currentBrushType);
    }
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************

std::unique_ptr<Brush> Canvas2D::initializeBrush(int brushType) {
    if (brushType == BRUSH_CONSTANT) {
        return std::make_unique<ConstantBrush>(settings.brushColor, settings.brushRadius);
    } else if (brushType == BRUSH_LINEAR) {
        return std::make_unique<LinearBrush>(settings.brushColor, settings.brushRadius);
    } else if (brushType == BRUSH_QUADRATIC) {
        return std::make_unique<QuadraticBrush>(settings.brushColor, settings.brushRadius);
    } else {
        return std::make_unique<SmudgeBrush>(settings.brushColor, settings.brushRadius);
    }
}

void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    std::cout << "Canvas2d::mouseDown() called" << std::endl;
    m_brush->brushDown(x, y, this);

    //    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit
}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;
    m_brush->brushDragged(x, y, this);
}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseUp() called" << std::endl;

    m_brush->brushUp(x, y, this);
}

// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

    RGBA *pix = data();
    int inputWidth = m_image->width();
    int inputHeight = m_image->height();

    // prepare a buffer to store the input image converted to a 0-1 scale
    std::vector<glm::vec3> inputVector(inputWidth * inputHeight);
    glm::vec3 *input = inputVector.data();

    for (size_t i = 0; i < inputWidth * inputHeight; i++) {
        input[i].r = pix[i].r / 255.0;
        input[i].g = pix[i].g / 255.0;
        input[i].b = pix[i].b / 255.0;
    }

    // determine the dimensions of the output image
    int outputWidth =
        round(settings.filterType == FILTER_SCALE ? inputWidth * settings.scaleX : inputWidth);
    int outputHeight =
        round(settings.filterType == FILTER_SCALE ? inputHeight * settings.scaleY : inputHeight);
    int outputSize = outputWidth * outputHeight;

    // create a buffer to store the output image
    std::vector<glm::vec3> outputVector(outputSize);
    glm::vec3 *output = outputVector.data();

    if (settings.filterType == FILTER_BLUR) {
        gaussianBlur(inputWidth, inputHeight, input, output, settings.blurRadius);
    } else if (settings.filterType == FILTER_EDGE_DETECT) {
        edgeDetect(inputWidth, inputHeight, input, output);
    } else if (settings.filterType == FILTER_SCALE) {
        // Create a buffer to store the intermediate scaling result
        std::vector<glm::vec3> scaledVector(outputWidth * inputHeight);
        glm::vec3 *scaled = scaledVector.data();

        // perform the scaling
        scaleImageX(
            input, inputWidth, inputHeight, settings.scaleX, scaled, outputWidth, inputHeight);
        scaleImageY(
            scaled, outputWidth, inputHeight, settings.scaleY, output, outputWidth, outputHeight);

        // resize the image to the new size
        resize(outputWidth, outputHeight);
        pix = data();

    } else {
        return;
    }

    // copy the `output` buffer to the canvas, converting from 0-1 to 0-255 and clamping
    for (size_t i = 0; i < outputSize; i++) {
        pix[i] = RGBA(std::clamp(output[i].r * 255.0, 0.0, 255.0),
                      std::clamp(output[i].g * 255.0, 0.0, 255.0),
                      std::clamp(output[i].b * 255.0, 0.0, 255.0),
                      255);
    }

    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) { m_rayScene.reset(scene); }

void Canvas2D::renderImage(CS123SceneCameraData *camera, int width, int height) {
    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.

        // load textures into memory
        std::vector<std::tuple<CS123ScenePrimitive, glm::mat4x4>> primitives =
            m_rayScene->getPrimitives();
        for (int i = 0; i < primitives.size(); i++) {
            CS123ScenePrimitive *primitive = m_rayScene->getPrimitive(i);

            if (primitive->material.textureMap.isUsed) {
                QString filename = QString::fromStdString(primitive->material.textureMap.filename);
                m_textures.push_back(std::make_unique<QImage>(filename));
                primitive->material.textureMap.handle = m_textures[m_textures.size() - 1].get();
            }
        }
        this->m_rayScene->loadEvironmentMap();

        // Resize the canvas
        resize(width, height);

        // Get the canvas data
        RGBA *pix = data();

        // Regardless of which ray tracing function we call below, we'll need some common pieces
        // of data that let us convert from camera space to world space. Calculate them now
        const float viewAngleHeight = camera->heightAngle;
        const float aspectRatio = (float)width / (float)height;
        const float viewAngleWidth = aspectRatio * viewAngleHeight;

        glm::vec3 w = glm::normalize(glm::vec3(-1.0f * camera->look));
        glm::vec3 v = glm::normalize(
            glm::vec3(glm::vec3(camera->up) - glm::dot(camera->up, glm::vec4(w, 0.0f)) * w));
        glm::vec3 u = glm::normalize(glm::cross(v, w));

        glm::mat4 cameraRotationMatrix = glm::transpose(
            glm::mat4(glm::vec4(u, 0), glm::vec4(v, 0), glm::vec4(w, 0), glm::vec4(0, 0, 0, 1)));
        glm::mat4 cameraTranslationMatrix =
            glm::mat4(glm::vec4(1, 0, 0, 0),
                      glm::vec4(0, 1, 0, 0),
                      glm::vec4(0, 0, 1, 0),
                      glm::vec4(-1.0f * (glm::vec3(camera->pos)), 1.0f));

        glm::mat4 cameraViewMatrix = cameraRotationMatrix * cameraTranslationMatrix;

        // Figure out which ray tracing function to call based on settings
        if (settings.useMultiThreading) {
            // If we're multithreading, we need to pack all the arguments to the ray trace call
            // up into a tuple
            auto args = std::vector<std::tuple<int,
                                               int,
                                               int,
                                               RGBA *,
                                               RayScene *,
                                               Canvas2D *,
                                               float,
                                               float,
                                               glm::mat4>>();
            args.reserve(height);
            // We want one thread to process each scan line, so we wrap up arguments and make a
            // vector that has one element for each line of hte image
            for (int row = 0; row < height; row++) {
                args.push_back(std::make_tuple(row,
                                               width,
                                               height,
                                               pix,
                                               m_rayScene.get(),
                                               this,
                                               viewAngleHeight,
                                               viewAngleWidth,
                                               cameraViewMatrix));
            }

            // Check if we're supersampling and call the appropriate function
            if (settings.useSuperSampling) {
                QtConcurrent::blockingMapped(args, RayTracer::traceScanLineSuperSampled);
                update();
            } else {
                QtConcurrent::blockingMapped(args, RayTracer::traceScanLine);
                update();
            }
        } else {
            // For each pixel in the image, ray trace and set the canvas to the resulting color
            for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                    // Calculate the screen space coordinate in the middle of the pixel
                    pix[row * width + col] = RayTracer::traceScreenSpacePoint(
                        2 * K * tan(glm::radians(viewAngleWidth) / 2.0) *
                            ((col + 0.5) / width - 0.5),
                        2 * K * tan(glm::radians(viewAngleHeight) / 2.0) *
                            (0.5 - ((row + 0.5) / height)),
                        cameraViewMatrix,
                        m_rayScene.get());
                }

                // Update the canvas after each scan line
                update();
                QCoreApplication::processEvents();
            }
        }
    }
}

void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}
