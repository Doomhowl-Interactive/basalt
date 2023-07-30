#pragma once
#include "basalt_math_tweening.hpp"

#ifndef PI
# define PI 3.14159265358979323846
#endif

// TODO: Add equality operators
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
    Rect operator*(int scalar);
    Rect operator/(int scalar);
    Rect& operator*=(int scalar);
    Rect& operator/=(int scalar);

    Point origin();
    Point center();

    bool overlaps(RectF with);
    bool overlaps(Rect with);

    bool inside(RectF other);
    bool inside(Rect other);

    Rect ScaleFromCenter(float scale);
};

struct RectF {
    float x;
    float y;
    float width;
    float height;

    operator Rect() const;
    RectF operator*(float scalar);
    RectF operator/(float scalar);
    RectF& operator*=(float scalar);
    RectF& operator/=(float scalar);

    Vec2 origin();
    Vec2 center();

    bool overlaps(RectF with);
    bool overlaps(Rect with);

    bool inside(RectF other);
    bool inside(Rect other);

    bool contains(Point other);
    bool contains(Vec2 other);

    RectF ScaleFromCenter(float scale);
};

struct Point {
    int x;
    int y;

    operator Vec2() const;
    bool operator==(const Point& other);
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
    Point& operator*=(const Point& other);
    Point& operator/=(const Point& other);
    Point& operator*=(int scalar);
    Point& operator/=(int scalar);

    Point operator+(const Point& other);
    Point operator-(const Point& other);
    Point operator*(const Point& other);
    Point operator/(const Point& other);
    Point operator*(int scalar);
    Point operator/(int scalar);

    bool inside(RectF other);
    bool inside(Rect other);
};

struct Vec2 {
    float x;
    float y;
    float z;

    operator Point() const;
    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator/=(const Vec2& other);
    Vec2& operator*=(float scalar);
    Vec2& operator/=(float scalar);

    Vec2 operator+(const Vec2& other);
    Vec2 operator-(const Vec2& other);
    Vec2 operator*(const Vec2& other);
    Vec2 operator/(const Vec2& other);
    Vec2 operator*(float scalar);
    Vec2 operator/(float scalar);

    bool inside(RectF other);
    bool inside(Rect other);

    Vec2 normalize();
    float magnitude();  // pythagorem theorem

    Vec2 directionTowards(Vec2 dest);

    float distance(Vec2 dest);
    float distanceSquared(Vec2 dest);
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

// TODO: Inconstent parameters with Clamp!!!
template <typename T> T Lerp(T start, T end, float percent)
{
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