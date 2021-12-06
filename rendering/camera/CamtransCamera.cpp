/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

// rotate `in` about `k` by `theta` radians
glm::vec4 rodrigues(glm::vec4 k, glm::vec4 in, float theta) {
    theta = theta * 3;
    glm::vec3 in_3 = glm::vec3(in);
    glm::vec3 k_3 = glm::vec3(k);

    glm::vec3 rotated = in_3 * cos(theta) + glm::cross(k_3, in_3) * glm::sin(theta) +
                        k_3 * (glm::dot(k_3, in_3)) * (1 - cos(theta));

    return glm::vec4(rotated, 0);
}

CamtransCamera::CamtransCamera() {
    setClip(1, 30);

    setHeightAngle(60);
    setAspectRatio(1);

    orientLook(glm::vec4(2, 2, 2, 1), glm::vec4(-2, -2, -2, 0), glm::vec4(0, 1, 0, 0));
}

void CamtransCamera::setAspectRatio(float a) {
    m_aspectRatio = a;
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const { return m_rotationMatrix * m_translationMatrix; }

glm::mat4x4 CamtransCamera::getScaleMatrix() const { return this->m_scaleMatrix; }

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return this->m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const { return this->m_eye; }

glm::vec4 CamtransCamera::getLook() const { return -this->m_w; }

glm::vec4 CamtransCamera::getU() const { return this->m_u; };
glm::vec4 CamtransCamera::getV() const { return this->m_v; };
glm::vec4 CamtransCamera::getW() const { return this->m_w; };

glm::vec4 CamtransCamera::getUp() const { return this->m_up; }

float CamtransCamera::getAspectRatio() const { return this->m_aspectRatio; }

float CamtransCamera::getHeightAngle() const { return glm::degrees(this->m_thetaH); }

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;

    m_w = glm::normalize(-look);
    m_v = glm::normalize(m_up - glm::dot(m_up, m_w) * m_w);
    m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0);

    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    h = glm::radians(h);

    m_thetaH = h;

    m_thetaW = glm::atan(m_aspectRatio * tan(m_thetaH / 2)) * 2;

    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye += v;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...

    auto theta = glm::radians(degrees);

    glm::vec4 rot = m_u;
    m_v = rodrigues(rot, m_v, theta);
    m_w = rodrigues(rot, m_w, theta);

    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...

    auto theta = glm::radians(degrees);

    glm::vec4 rot = m_v;
    m_u = rodrigues(rot, m_u, theta);
    m_w = rodrigues(rot, m_w, theta);

    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...

    auto theta = glm::radians(degrees);

    glm::vec4 rot = m_w;
    m_u = rodrigues(rot, m_u, theta);
    m_v = rodrigues(rot, m_v, theta);

    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix() {
    this->updateScaleMatrix();
    this->updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {
    auto c = -m_near / m_far;
    m_perspectiveTransformation =
        glm::transpose(glm::mat4x4(glm::vec4(1, 0, 0, 0),
                                   glm::vec4(0, 1, 0, 0),
                                   glm::vec4(0, 0, -1 / (1 + c), c / (1 + c)),
                                   glm::vec4(0, 0, -1, 0)));
}

void CamtransCamera::updateScaleMatrix() {
    auto tanThetaHOver2 = tan(m_thetaH / 2);
    auto tanThetaWOver2 = m_aspectRatio * tanThetaHOver2;

    this->m_scaleMatrix = glm::transpose(glm::mat4(glm::vec4(1 / (m_far * tanThetaWOver2), 0, 0, 0),
                                                   glm::vec4(0, 1 / (m_far * tanThetaHOver2), 0, 0),
                                                   glm::vec4(0, 0, 1 / m_far, 0),
                                                   glm::vec4(0, 0, 0, 1)));
}

void CamtransCamera::updateViewMatrix() {
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix() {
    this->m_rotationMatrix = glm::mat4(glm::vec4(this->m_u.x, this->m_v.x, this->m_w.x, 0.0f),
                                       glm::vec4(this->m_u.y, this->m_v.y, this->m_w.y, 0.0f),
                                       glm::vec4(this->m_u.z, this->m_v.z, this->m_w.z, 0.0f),
                                       glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void CamtransCamera::updateTranslationMatrix() {
    auto P_n = m_eye - this->m_near * this->m_w;

    this->m_translationMatrix = glm::transpose(glm::mat4(glm::vec4(1, 0, 0, -P_n.x),
                                                         glm::vec4(0, 1, 0, -P_n.y),
                                                         glm::vec4(0, 0, 1, -P_n.z),
                                                         glm::vec4(0, 0, 0, 1.0f)));
}
