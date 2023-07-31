#include <stdlib.h>
#include <iostream>

#include "basalt_math.hpp"

inline Vec2 RectFOrigin(RectF rectf)
{
    Vec2 origin = {
        rectf.x,
        rectf.y,
    };
    return origin;
}

inline Vec2 RectFCenter(RectF rectf)
{
    Vec2 res = {
        rectf.x + rectf.width * 0.5f,
        rectf.y + rectf.height * 0.5f,
    };
    return res;
}

inline Point RectOrigin(Rect rect)
{
    Point origin = {
        rect.x,
        rect.y,
    };
    return origin;
}

inline Point RectCenter(Rect rect)
{
    Point res = {
        rect.x + rect.width / 2,
        rect.y + rect.height / 2,
    };
    return res;
}

inline Rect RectFToRect(RectF rectf)
{
    Rect rect = {
        (int)rectf.x,
        (int)rectf.y,
        (int)rectf.width,
        (int)rectf.height,
    };
    return rect;
}

Rect::operator RectF() const
{
    return {
        (float)x,
        (float)y,
        (float)width,
        (float)height,
    };
    ;
}

Rect Rect::operator*(int scalar)
{
    return {
        x * scalar,
        y * scalar,
        width * scalar,
        height * scalar,
    };
}

Rect Rect::operator/(int scalar)
{
    return {
        x / scalar,
        y / scalar,
        width / scalar,
        height / scalar,
    };
}

Rect& Rect::operator*=(int scalar)
{
    x *= scalar;
    y *= scalar;
    width *= scalar;
    height *= scalar;
    return *this;
}

Rect& Rect::operator/=(int scalar)
{
    x /= scalar;
    y /= scalar;
    width /= scalar;
    height /= scalar;
    return *this;
}

Point Rect::origin()
{
    Point p2 = {
        x,
        y,
    };
    return p2;
}

Point Rect::center()
{
    Point p2 = {
        x + width / 2,
        y + height / 2,
    };
    return p2;
}

bool Rect::overlaps(RectF with)
{
    return !(x < with.x || y < with.y || x + width > with.x + with.width
             || y + height > with.y + with.height);
}

bool Rect::overlaps(Rect with)
{
    return !(x < with.x || y < with.y || x + width > with.x + with.width
             || y + height > with.y + with.height);
}

bool Rect::inside(RectF other)
{
    return x > other.x && y > other.y && x + width < other.x + other.width
           && y + height < other.y + other.height;
}

bool Rect::inside(Rect other)
{
    return x > other.x && y > other.y && x + width < other.x + other.width
           && y + height < other.y + other.height;
}

Rect Rect::ScaleFromCenter(float scale)
{
    Rect r = {
        (int)(x + width * (1 - scale) / 2),
        (int)(y + height * (1 - scale) / 2),
        (int)(width * scale),
        (int)(height * scale),
    };
    return r;
}

bool Rect::operator==(const Rect& other) const
{
    return x == other.x && y == other.y && width == other.width && height == other.height;
}

bool Rect::operator!=(const Rect& other) const
{
    return x != other.x || y != other.y || width != other.width || height != other.height;
}

Rect Rect::operator-() const
{
    return {
        -x,
        -y,
        -width,
        -height,
    };
}

RectF::operator Rect() const
{
    return { (int)x, (int)y, (int)width, (int)height };
}

bool RectF::operator==(const RectF& other) const
{
    return x == other.x && y == other.y && width == other.width && height == other.height;
}

bool RectF::operator!=(const RectF& other) const
{
    return x != other.x || y != other.y || width != other.width || height != other.height;
}

RectF RectF::operator-() const
{
    return {
        -x,
        -y,
        -width,
        -height,
    };
}

RectF RectF::operator*(float scalar)
{
    return {
        x * scalar,
        y * scalar,
        width * scalar,
        height * scalar,
    };
}

RectF RectF::operator/(float scalar)
{
    return {
        x / scalar,
        y / scalar,
        width / scalar,
        height / scalar,
    };
}

RectF& RectF::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    width *= scalar;
    height *= scalar;
    return *this;
}

RectF& RectF::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    width /= scalar;
    height /= scalar;
    return *this;
}

RectF RectF::ScaleFromCenter(float scale)
{
    RectF r = {
        x + width * (1 - scale) / 2,
        y + height * (1 - scale) / 2,
        width * scale,
        height * scale,
    };
    return r;
}

Vec2 RectF::origin()
{
    Vec2 v2 = {
        x,
        y,
    };
    return v2;
}

Vec2 RectF::center()
{
    Vec2 v2 = {
        x + width * 0.5f,
        y + height * 0.5f,
    };
    return v2;
}

bool RectF::overlaps(RectF with)
{
    return !(x < with.x || y < with.y || x + width > with.x + with.width
             || y + height > with.y + with.height);
}

bool RectF::overlaps(Rect with)
{
    return !(x < with.x || y < with.y || x + width > with.x + with.width
             || y + height > with.y + with.height);
}

bool RectF::inside(RectF other)
{
    return x > other.x && x + width < other.x + other.width && y > other.y
           && y + height < other.y + other.height;
}

bool RectF::inside(Rect other)
{
    return inside((RectF)other);
}

bool RectF::contains(Point other)
{
    return other.inside(*this);
}

bool RectF::contains(Vec2 other)
{
    return other.inside(*this);
}

Point::operator Vec2() const
{
    return { (float)x, (float)y };
}

bool Point::operator==(const Point& other) const
{
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const
{
    return x != other.x || y != other.y;
}

Point& Point::operator+=(const Point& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Point& Point::operator-=(const Point& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Point& Point::operator*=(const Point& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Point& Point::operator/=(const Point& other)
{
    x /= other.x;
    y /= other.y;
    return *this;
}

Point& Point::operator*=(int scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Point& Point::operator/=(int scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Point Point::operator+(const Point& other)
{
    Point p2 = {
        x + other.x,
        y + other.y,
    };
    return p2;
}

Point Point::operator-(const Point& other)
{
    Point p2 = {
        x - other.x,
        y - other.y,
    };
    return p2;
}

Point Point::operator*(const Point& other)
{
    Point p2 = {
        x * other.x,
        y * other.y,
    };
    return p2;
}

Point Point::operator/(const Point& other)
{
    Point p2 = {
        x / other.x,
        y / other.y,
    };
    return p2;
}

Point Point::operator-() const
{
    return { -x, -y };
}

Point Point::operator*(int scalar)
{
    Point p2 = { x * scalar, y * scalar };
    return p2;
}

Point Point::operator/(int scalar)
{
    Point p2 = { x / scalar, y / scalar };
    return p2;
}

bool Point::inside(RectF other)
{
    return x >= other.x && x <= other.x + other.width && y >= other.y
           && y <= other.y + other.height;
}

bool Point::inside(Rect other)
{
    return x >= other.x && x <= other.x + other.width && y >= other.y
           && y <= other.y + other.height;
}

// vector2

Vec2::operator Point() const
{
    return { (int)x, (int)y };
}

bool Vec2::operator==(const Vec2& other) const
{
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return x != other.x || y != other.y;
}

Vec2& Vec2::operator+=(const Vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(const Vec2& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Vec2& Vec2::operator/=(const Vec2& other)
{
    x /= other.x;
    y /= other.y;
    return *this;
}

Vec2& Vec2::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2& Vec2::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

Vec2 Vec2::operator-() const
{
    return { -x, -y };
}

Vec2 Vec2::operator+(const Vec2& other)
{
    Vec2 v2 = {
        x + other.x,
        y + other.y,
    };
    return v2;
}

Vec2 Vec2::operator-(const Vec2& other)
{
    Vec2 v2 = {
        x - other.x,
        y - other.y,
    };
    return v2;
}

Vec2 Vec2::operator*(const Vec2& other)
{
    Vec2 v2 = {
        x * other.x,
        y * other.y,
    };
    return v2;
}

Vec2 Vec2::operator/(const Vec2& other)
{
    Vec2 v2 = {
        x / other.x,
        y / other.y,
    };
    return v2;
}

Vec2 Vec2::operator/(float scalar)
{
    Vec2 v2 = {
        x / scalar,
        y / scalar,
    };
    return v2;
}

Vec2 Vec2::operator*(float scalar)
{
    Vec2 v2 = {
        x * scalar,
        y * scalar,
    };
    return v2;
}

bool Vec2::inside(RectF other)
{
    return x >= other.x && x <= other.x + other.width && y >= other.y
           && y <= other.y + other.height;
}

bool Vec2::inside(Rect other)
{
    return x >= other.x && x <= other.x + other.width && y >= other.y
           && y <= other.y + other.height;
}

Vec2 Vec2::normalize()
{
    float mag = magnitude();
    Vec2 norm = { x / mag, y / mag };
    return norm;
}

float Vec2::magnitude()
{
    return sqrt(x * x + y * y);
}

Vec2 Vec2::directionTowards(Vec2 dest)
{
    Vec2 diff = dest - *this;
    return diff.normalize();
}

float Vec2::distanceSquared(Vec2 dest)
{
    float dist = ((dest.x - x) * (dest.x - x)) + ((dest.y - y) * (dest.y - y));
    return dist;
}

float Vec2::distance(Vec2 dest)
{
    float distSquared = distanceSquared(dest);
    float distRoot = sqrtf(distSquared);
    return distRoot;
}

void SetRandomSeed(unsigned int seed)
{
    srand(seed);
}

int GetRandomRange(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

int GetRandomNumber()
{
    return rand();
}

bool IsLittleEndian()
{
    volatile unsigned int i = 0x01234567;
    bool littleEndian = *((unsigned char*)(&i)) == 0x67;
    return littleEndian;
}
