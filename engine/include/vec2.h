#pragma once
#include <cmath>

struct vec2 {
    vec2() : x(0), y(0) {
    }

    vec2(const float x, const float y) : x(x), y(y) {
    }

    vec2(const float f) : x(f), y(f) {
    }

    vec2(const int x, const int y) : x((float) x), y((float) y) {
    }

    vec2(const int i) : x((float) i), y((float) i) {
    }

    float x = 0.0f, y = 0.0f;

    vec2 operator+(const vec2 &rhs) const { return vec2(x + rhs.x, y + rhs.y); }
    vec2 operator-(const vec2 &rhs) const { return vec2(x - rhs.x, y - rhs.y); }
    vec2 operator*(const vec2 &rhs) const { return vec2(x * rhs.x, y * rhs.y); }
    vec2 operator/(const vec2 &rhs) const { return vec2(x / rhs.x, y / rhs.y); }

    vec2 &operator+=(const vec2 &rhs) {
        x += rhs.x, y += rhs.y;
        return *this;
    }

    vec2 &operator-=(const vec2 &rhs) {
        x -= rhs.x, y -= rhs.y;
        return *this;
    }

    vec2 &operator*=(const vec2 &rhs) {
        x *= rhs.x, y *= rhs.y;
        return *this;
    }

    bool operator==(const vec2 &rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const vec2 &rhs) const { return x != rhs.x || y != rhs.y; }

    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSq() const { return x * x + y * y; }

    vec2 normalized() const { return vec2(x / length(), y / length()); }

    float dot(const vec2 &rhs) const { return x * rhs.x + y * rhs.y; }
};
