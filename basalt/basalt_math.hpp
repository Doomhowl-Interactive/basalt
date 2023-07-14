#pragma once

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

    Point origin();
    Point center();

    bool overlaps(RectF with);
    bool overlaps(Rect with);

    bool inside(RectF other);
    bool inside(Rect other);
};

struct RectF {
    float x;
    float y;
    float width;
    float height;

    operator Rect() const;

    Vec2 origin();
    Vec2 center();

    bool overlaps(RectF with);
    bool overlaps(Rect with);

    bool inside(RectF other);
    bool inside(Rect other);

    bool contains(Point other);
    bool contains(Vec2 other);
};

struct Point {
    int x;
    int y;

    operator Vec2() const;
    Point& operator+=(const Point& other);
    Point& operator-=(const Point& other);
    Point& operator*=(const Point& other);
    Point& operator/=(const Point& other);

    Point operator+(const Point& other);
    Point operator-(const Point& other);
    Point operator*(const Point& other);
    Point operator/(const Point& other);
    Point operator*(int scalar);
    Point operator/(int scalar);

    bool inside(RectF other);
    bool inside(Rect other);
};

struct Size {
    unsigned int width;
    unsigned int height;
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
extern void SetRandomSeed(unsigned int seed);
extern int GetRandomRange(int min, int max);
extern int GetRandomNumber();

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