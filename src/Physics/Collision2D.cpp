﻿
#include <vector>
#include <algorithm>
#include "LarvaEngine/Physics/Collision2D.h"

// ===== LineSegment2D ===== //

LineSegment2D::LineSegment2D(const Vector2& start, const Vector2& end)
    : _start(start), _end(end) {}

/**
 * 
 */
Vector2 LineSegment2D::PointOnSegment(float t) const {
    return Vector2(_start) + t * Vector2(_end - _start);
}

float LineSegment2D::MinDistSq(const Vector2& point) const {
    Vector2 ab = _end - _start;
    Vector2 ap = point - _start;
    float proj = static_cast<float>(ap.Dot(ap, ab)) / ab.LengthSq();
    proj = Math::Clamp(proj, 0.0f, 1.0f);
    return (Vector2(point) - PointOnSegment(proj)).LengthSq();
}

float LineSegment2D::MinDistSq(const LineSegment2D& s) const {
    float minDistSq = std::numeric_limits<float>::max();

    for (float t1 = 0.0f; t1 <= 1.0f; t1 += 0.1f) {
        for (float t2 = 0.0f; t2 <= 1.0f; t2 += 0.1f) {
            float distSq = (PointOnSegment(t1) - s.PointOnSegment(t2)).LengthSq();
            minDistSq = Math::Min(minDistSq, distSq);
        }
    }

    return minDistSq;
}

float LineSegment2D::MinDistSq(const LineSegment2D& s1, const LineSegment2D& s2) {
    float minDistSq = std::numeric_limits<float>::max();
    for (float t1 = 0.0f; t1 <= 1.0f; t1 += 0.1f) {
        for (float t2 = 0.0f; t2 <= 1.0f; t2 += 0.1f) {
            float distSq = (s1.PointOnSegment(t1) - s2.PointOnSegment(t2)).LengthSq();
            minDistSq = Math::Min(minDistSq, distSq);
        }
    }
    return minDistSq;
}

Circle::Circle(const Vector2& center, float radius)
    : _center(center), _radius(radius) {}

bool Circle::Contains(const Vector2& point) const {
    return (point - _center).LengthSq() <= (_radius * _radius);
}


AABB2D::AABB2D(const Vector2& min, const Vector2& max)
    : _min(min), _max(max) {}

void AABB2D::UpdateMinMax(const Vector2& point) {
    _min.x = Math::Min(_min.x, point.x);
    _min.y = Math::Min(_min.y, point.y);
    _max.x = Math::Max(_max.x, point.x);
    _max.y = Math::Max(_max.y, point.y);
}

bool AABB2D::Contains(const Vector2& point) const {
    return (point.x >= _min.x && point.x <= _max.x &&
        point.y >= _min.y && point.y <= _max.y);
}

float AABB2D::MinDistSq(const Vector2& point) const {
    float dx = Math::Max(_min.x - point.x, 0.f);
    dx = Math::Max(dx, point.x - _max.x);
    float dy = Math::Max(_min.y - point.y, 0.f);
    dy = Math::Max(dy, point.y - _max.y);
    return static_cast<float>(dx * dx + dy * dy);
}

void AABB2D::MoveCenterTo(const Vector2& targetPoint) {
    Vector2 currentCenter = Center();
    Vector2 offset = targetPoint - currentCenter;

    _min += offset;
    _max += offset;
}

Vector2 AABB2D::Center() const {
    return Vector2((_min.x + _max.x) / 2, (_min.y + _max.y) / 2);
}


Capsule2D::Capsule2D(const Vector2& start, const Vector2& end, float radius)
    : _segment(start, end), _radius(radius) {}

Vector2 Capsule2D::PointOnSegment(float t) const {
    return Vector2(_segment.PointOnSegment(t));
}

bool Capsule2D::Contains(const Vector2& point) const {
    return _segment.MinDistSq(point) <= (_radius * _radius);
}


bool Intersect(const Circle& a, const Circle& b) {
    float distSq = (a._center - b._center).LengthSq();
    float radiusSum = a._radius + b._radius;
    return distSq <= (radiusSum * radiusSum);
}

bool Intersect(const AABB2D& a, const AABB2D& b) {
    return !(a._max.x < b._min.x || a._min.x > b._max.x ||
        a._max.y < b._min.y || a._min.y > b._max.y);
}

bool Intersect(const Capsule2D& a, const Capsule2D& b) {
    return (a._segment.MinDistSq(b._segment) <= (a._radius + b._radius) * (a._radius + b._radius));
}

bool Intersect(const Circle& c, const AABB2D& box) {
    return box.MinDistSq(c._center) <= (c._radius * c._radius);
}

bool TestSidePlane(float start, float end, float negd, const Vector2& norm,
    std::vector<std::pair<float, Vector2>>& out) {
    float denom = end - start;
    if (Math::NearZero(denom)) {
        return false;
    }
    else {
        float numer = -start + negd;
        float t = numer / denom;
        if (t >= 0.0f && t <= 1.0f) {
            out.emplace_back(t, norm);
            return true;
        }
        else {
            return false;
        }
    }
}

bool Intersect(const LineSegment2D& l, const AABB2D& b, float& outT, Vector2& outNorm) {
    std::vector<std::pair<float, Vector2>> tValues;

    // X軸面との交差テスト
    TestSidePlane(l._start.x, l._end.x, b._min.x, Vector2::NegUnitX, tValues);
    TestSidePlane(l._start.x, l._end.x, b._max.x, Vector2::UnitX, tValues);

    // Y軸面との交差テスト
    TestSidePlane(l._start.y, l._end.y, b._min.y, Vector2::NegUnitY, tValues);
    TestSidePlane(l._start.y, l._end.y, b._max.y, Vector2::UnitY, tValues);

    // t値で昇順にソート
    std::sort(tValues.begin(), tValues.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
        });

    // 各交差点がAABB内にあるかをテスト
    for (auto& t : tValues) {
        Vector2 point = l.PointOnSegment(t.first);
        if (b.Contains(Vector2(point))) {
            outT = t.first;
            outNorm = t.second;
            return true;
        }
    }
    return false; // 交差なし
}
