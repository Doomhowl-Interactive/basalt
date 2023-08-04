#pragma once
#include "basalt_math_tweening.hpp"

#ifndef PI
# define PI 3.14159265358979323846
#endif

// TODO: Add negation operator

struct RectF;
struct Vec2;
struct Point;

struct Rect {
    int x;
    int y;
    int width;
    int height;

    operator RectF() const;
    bool operator==(const Rect& other) const;
    bool operator!=(const Rect& other) const;

    Rect operator-() const;
    Rect operator*(int scalar) const;
    Rect operator/(int scalar) const;
    Rect& operator*=(int scalar);
    Rect& operator/=(int scalar);

    Point origin();
    Point center() const;

    bool overlaps(RectF with) const;
    bool overlaps(Rect with) const;

    bool inside(RectF other) const;
    bool inside(Rect other) const;

    Rect ScaleFromCenter(float scale) const;
};

struct RectF {
    float x;
    float y;
    float width;
    float height;

    operator Rect() const;
    bool operator==(const RectF& other) const;
    bool operator!=(const RectF& other) const;

    RectF operator-() const;
    RectF operator*(float scalar) const;
    RectF operator/(float scalar) const;
    RectF& operator*=(float scalar);
    RectF& operator/=(float scalar);

    Vec2 origin();
    Vec2 center() const;

    bool overlaps(RectF with) const;
    bool overlaps(Rect with) const;
    bool inside(RectF other) const;

    bool contains(Point other);
    bool contains(Vec2 other);

    RectF ScaleFromCenter(float scale) const;
};

struct Point {
    int x;
    int y;

    operator Vec2() const;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    Point operator-() const;
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);

    // TODO: also add these operators for Vec2
    Point& operator+=(int offset);
    Point& operator-=(int offset);

    Point& operator*=(const Point& other);
    Point& operator/=(const Point& other);
    Point& operator*=(int scalar);
    Point& operator/=(int scalar);

    Point operator+(const Point& other) const;
    Point operator-(const Point& other) const;
    Point operator*(const Point& other) const;
    Point operator/(const Point& other) const;

    // TODO: also add these operators for Vec2
    Point operator+(int offset) const;
    Point operator-(int offset) const;

    Point operator*(int scalar) const;
    Point operator/(int scalar) const;

    bool inside(RectF other) const;
    bool inside(Rect other) const;
};

struct Vec2 {
    float x;
    float y;
    float z;

    operator Point() const;
    bool operator==(const Vec2& other) const;
    bool operator!=(const Vec2& other) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator/=(const Vec2& other);
    Vec2& operator*=(float scalar);
    Vec2& operator/=(float scalar);

    Vec2 operator-() const;
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator/(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    bool inside(RectF other) const;
    bool inside(Rect other) const;

    Vec2 normalize();
    float magnitude() const;  // pythagorem theorem

    Vec2 directionTowards(Vec2 dest) const;

    float distance(Vec2 dest) const;
    float distanceSquared(Vec2 dest) const;
};

// RNG
void SetRandomSeed(unsigned int seed);
int GetRandomRange(int min, int max);
int GetRandomNumber();

bool IsLittleEndian();

// Math

template <typename T> T Clamp(T value, T min, T max)
{
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

template <typename T> T Lerp(float percent, T start, T end)
{
    if (start == end) {
        return start;
    }
    if (start > end) {
        return (start - percent * (start - end));
    }
    return (start + percent * (end - start));
}

template <typename T> T Sign(T a)
{
    if (a > 0) {
        return 1;
    } else if (a < 0) {
        return -1;
    } else {
        return 0;
    }
}

template <typename T> inline T Deg2Rad(T val)
{
    return val * PI / 180;
}

template <typename T> inline T Rad2Deg(T val)
{
    return val * 180.0 / PI;
}