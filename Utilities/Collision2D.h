#pragma once
#include "Math.h"
#include <vector>

struct LineSegment2D
{
    LineSegment2D(const Vector2Int& start, const Vector2Int& end);
    Vector2 PointOnSegment(float t) const;
    float MinDistSq(const Vector2Int& point) const;
    float MinDistSq(const LineSegment2D& s) const;
    static float MinDistSq(const LineSegment2D& s1, const LineSegment2D& s2);

    Vector2Int _start;
    Vector2Int _end;
};

struct Circle
{
    Circle(const Vector2Int& center, float radius);
    bool Contains(const Vector2Int& point) const;

    Vector2Int _center;
    float _radius;
};

struct AABB2D
{
    AABB2D(const Vector2Int& min, const Vector2Int& max);
    void UpdateMinMax(const Vector2Int& point);
    bool Contains(const Vector2Int& point) const;
    float MinDistSq(const Vector2Int& point) const;
    void MoveCenterTo(const Vector2Int& targetPoint);
    Vector2Int Center() const;

    Vector2Int _min;
    Vector2Int _max;
};

struct Capsule2D
{
    Capsule2D(const Vector2Int& start, const Vector2Int& end, float radius);
    Vector2Int PointOnSegment(float t) const;
    bool Contains(const Vector2Int& point) const;

    LineSegment2D _segment;
    float _radius;
};

struct ConvexPolygon2D
{
    bool Contains(const Vector2Int& point) const;
    std::vector<Vector2Int> _vertices;
};

// Intersection functions
bool Intersect(const Circle& a, const Circle& b);
bool Intersect(const AABB2D& a, const AABB2D& b);
bool Intersect(const Capsule2D& a, const Capsule2D& b);
bool Intersect(const Circle& c, const AABB2D& box);

//bool Intersect(const LineSegment2D& l, const Circle& c, float& outT);

bool TestSidePlane(float start, float end, float negd, const Vector2Int& norm,std::vector<std::pair<float, Vector2Int>>& out);
bool Intersect(const LineSegment2D& l, const AABB2D& b, float& outT, Vector2Int& outNorm);
