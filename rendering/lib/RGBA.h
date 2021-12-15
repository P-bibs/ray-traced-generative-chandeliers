#ifndef RGBA_H
#define RGBA_H

#include "glm/gtx/string_cast.hpp"
#include <glm/glm.hpp>

struct RGBAfloat;
// A structure for a color.  Each channel is 8 bits [0-255].
struct RGBA {
    RGBA() : r(0), g(0), b(0), a(255) {}
    RGBA(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255)
        : r(red), g(green), b(blue), a(alpha) {}
    RGBA(RGBAfloat f);

    static glm::vec3 toVec(RGBA color) { return glm::vec3(color.r, color.g, color.b); }
    // C++ TIP:
    // A union struct. Essentially, this makes b <==> channels[0],
    // g <==> channels[1], etc. In other words: b, g, r, and a are
    // stored at the same memory location as channels[4]. Note that
    // sizeof(b)+sizeof(g)+sizeof(r)+sizeof(a) = sizeof(channels)
    // so the memory overlaps **exactly**.
    //
    // You might want to take advantage of union structs in later
    // assignments, although we don't require or expect you to do so.
    //

    union {
        struct { unsigned char r, g, b, a; };
        unsigned char channels[4];
    };

    RGBA operator+(const RGBA &c1) const;
    RGBA operator-(const RGBA &c1) const;
    RGBA operator*(const RGBA &c1) const;
    RGBA operator/(const RGBA &c1) const;

    friend bool operator==(const RGBA &c1, const RGBA &c2);
    friend bool operator!=(const RGBA &c1, const RGBA &c2);

};

struct RGBAfloat {
    RGBAfloat() {
        r = 0;
        g = 0;
        b = 0;
        a = 1.0;
    }
    RGBAfloat(glm::vec3 color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = 1.0;
    }
    RGBAfloat(glm::vec4 color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }
    RGBAfloat(RGBA color) {
        this->r = color.r/255.0;
        this->g = color.g/255.0;
        this->b = color.b/255.0;
        this->a = color.a/255.0;
    }
    RGBAfloat(RGBAfloat &color) {
        this->r = color.r;
        this->g = color.g;
        this->b = color.b;
        this->a = color.a;
    }
    RGBAfloat(float r, float g, float b, float a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    float r, g, b, a;

    static glm::vec3 toVec(RGBAfloat color) { return glm::vec3(color.r, color.g, color.b); }

    RGBAfloat operator+(const RGBAfloat &c1) const;
    RGBAfloat operator-(const RGBAfloat &c1) const;
    RGBAfloat operator*(const RGBAfloat &c1) const;
    RGBAfloat operator/(const RGBAfloat &c1) const;

    friend bool operator==(const RGBAfloat &c1, const RGBAfloat &c2);
    friend bool operator!=(const RGBAfloat &c1, const RGBAfloat &c2);
};

#endif // RGBA_H
