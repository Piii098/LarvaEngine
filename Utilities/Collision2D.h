#pragma once
#include "Math.h"
#include <vector>

struct LineSegment2D
{
    LineSegment2D(const Vector2& start, const Vector2& end);
    Vector2 PointOnSegment(float t) const;
    float MinDistSq(const Vector2& point) const;
    float MinDistSq(const LineSegment2D& s) const;
    static float MinDistSq(const LineSegment2D& s1, const LineSegment2D& s2);

    Vector2 _start;
    Vector2 _end;
};

struct Circle
{
    Circle(const Vector2& center, float radius);
    bool Contains(const Vector2& point) const;

    Vector2 _center;
    float _radius;
};

struct AABB2D
{
    AABB2D(const Vector2& min, const Vector2& max);
    void UpdateMinMax(const Vector2& point);
    bool Contains(const Vector2& point) const;
    float MinDistSq(const Vector2& point) const;

    Vector2 _min;
    Vector2 _max;
};

struct Capsule2D
{
    Capsule2D(const Vector2& start, const Vector2& end, float radius);
    Vector2 PointOnSegment(float t) const;
    bool Contains(const Vector2& point) const;

    LineSegment2D _segment;
    float _radius;
};

struct ConvexPolygon2D
{
    bool Contains(const Vector2& point) const;
    std::vector<Vector2> _vertices;
};

// Intersection functions
bool Intersect(const Circle& a, const Circle& b);
bool Intersect(const AABB2D& a, const AABB2D& b);
bool Intersect(const Capsule2D& a, const Capsule2D& b);
bool Intersect(const Circle& c, const AABB2D& box);

//bool Intersect(const LineSegment2D& l, const Circle& c, float& outT);

bool TestSidePlane(float start, float end, float negd, const Vector2& norm,std::vector<std::pair<float, Vector2>>& out);
bool Intersect(const LineSegment2D& l, const AABB2D& b, float& outT, Vector2& outNorm);
