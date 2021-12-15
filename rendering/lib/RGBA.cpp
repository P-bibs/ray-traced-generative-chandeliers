#include "RGBA.h"

RGBA::RGBA(RGBAfloat rf) {
    this->r = glm::min(static_cast<int>(glm::max(rf.r,0.0f) * 255 + 0.5), 255);
    this->g = glm::min(static_cast<int>(glm::max(rf.g,0.0f) * 255 + 0.5), 255);
    this->b = glm::min(static_cast<int>(glm::max(rf.b,0.0f) * 255 + 0.5), 255);
    this->a = glm::min(static_cast<int>(glm::max(rf.a,0.0f) * 255 + 0.5), 255);
}

RGBA RGBA::operator+(const RGBA &that) const {
    return RGBA(this->r + that.r, this->g + that.g, this->b + that.b, this->a + that.a);
}

RGBA RGBA::operator-(const RGBA &that) const {
    return RGBA(this->r - that.r, this->g - that.g, this->b - that.b, this->a - that.a);
}

RGBA RGBA::operator*(const RGBA &that) const {
    return RGBA(this->r * that.r, this->g * that.g, this->b * that.b, this->a * that.a);
}

RGBA RGBA::operator/(const RGBA &that) const {
    return RGBA(this->r / that.r, this->g / that.g, this->b / that.b, this->a / that.a);
}

bool operator==(const RGBA &c1, const RGBA &c2) {
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b) && (c1.a == c2.a);
}

bool operator!=(const RGBA &c1, const RGBA &c2) {
    return !operator==(c1, c2);
}


RGBAfloat RGBAfloat::operator+(const RGBAfloat &that) const {
    return RGBAfloat(this->r + that.r, this->g + that.g, this->b + that.b, this->a + that.a);
}

RGBAfloat RGBAfloat::operator-(const RGBAfloat &that) const {
    return RGBAfloat(this->r - that.r, this->g - that.g, this->b - that.b, this->a - that.a);
}

RGBAfloat RGBAfloat::operator*(const RGBAfloat &that) const {
    return RGBAfloat(this->r * that.r, this->g * that.g, this->b * that.b, this->a * that.a);
}

RGBAfloat RGBAfloat::operator/(const RGBAfloat &that) const {
    return RGBAfloat(this->r / that.r, this->g / that.g, this->b / that.b, this->a / that.a);
}

bool operator==(const RGBAfloat &c1, const RGBAfloat &c2) {
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b) && (c1.a == c2.a);
}

bool operator!=(const RGBAfloat &c1, const RGBAfloat &c2) {
    return !operator==(c1, c2);
}
