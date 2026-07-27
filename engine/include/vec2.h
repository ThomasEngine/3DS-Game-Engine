#pragma once
#include <cmath>

struct Vec2 {
    Vec2() : x(0), y(0) {}
    Vec2(const float x, const float y) : x(x), y(y) {}
    Vec2(const float f) : x(f), y(f) {}
    Vec2(const int x, const int y) : x((float)x), y((float)y) {}
    Vec2(const int i) : x((float)i), y((float)i) {}

    float x = 0.0f, y = 0.0f;

    Vec2 operator+(const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
    Vec2 operator-(const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
    Vec2 operator*(const Vec2& rhs) const { return Vec2(x * rhs.x, y * rhs.y); }
    Vec2 operator/(const Vec2& rhs) const { return Vec2(x / rhs.x, y / rhs.y); }
    Vec2& operator+=(const Vec2& rhs) { x += rhs.x, y += rhs.y; return *this; }
    Vec2& operator-=(const Vec2& rhs) { x -= rhs.x, y -= rhs.y; return *this; }
    Vec2& operator*=(const Vec2& rhs) { x *= rhs.x, y *= rhs.y; return *this; }
    bool operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vec2& rhs) const { return x != rhs.x || y != rhs.y; }

    float length() const { return std::sqrt(x * x + y * y); }
    float lengthSq() const { return x * x + y * y; }

    Vec2 normalized() const { return Vec2(x / length(), y / length()); }

    float dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }
};