// ----------------------------------------------------------------
// From Game Progra_ming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "LarvaEngine/Core/Utilities/Math.h"
#include <vector>

struct LineSegment3D
{
	LineSegment3D(const Vector3& start, const Vector3& end);
	// Get point along segment where 0 <= t <= 1
	Vector3 PointOnSegment(float t) const;
	// Get minimum distance squared between point and line segment
	float MinDistSq(const Vector3& point) const;
	// Get MinDistSq between two line segments
	static float MinDistSq(const LineSegment3D& s1, const LineSegment3D& s2);

	Vector3 mStart;
	Vector3 mEnd;
};

struct Plane3D
{
	Plane3D(const Vector3& normal, float d);
	// Construct plane from three points
	Plane3D(const Vector3& a, const Vector3& b, const Vector3& c);
	// Get the signed distance between the point and the plane
	float SignedDist(const Vector3& point) const;

	Vector3 mNormal;
	float mD;
};

struct Sphere3D
{
	Sphere3D(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;

	Vector3 mCenter;
	float mRadius;
};

struct AABB3D
{
	AABB3D(const Vector3& min, const Vector3& max);
	// Update min and max accounting for this point
	// (used when loading a model)
	void UpdateMinMax(const Vector3& point);
	// Rotated by a quaternion
	void Rotate(const Quaternion& q);
	bool Contains(const Vector3& point) const;
	float MinDistSq(const Vector3& point) const;

	void MoveCenterTo(const Vector3& center);

	Vector3 _min;
	Vector3 _max;
};

struct OBB3D
{
	Vector3 mCenter;
	Quaternion mRotation;
	Vector3 mExtents;
};

struct Capsule3D
{
	Capsule3D(const Vector3& start, const Vector3& end, float radius);
	// Get point along segment where 0 <= t <= 1
	Vector3 PointOnSegment(float t) const;
	bool Contains(const Vector3& point) const;

	LineSegment3D mSegment;
	float mRadius;
};

struct ConvexPolygon3D
{
	bool Contains(const Vector2& point) const;
	// Vertices have a clockwise ordering
	std::vector<Vector2> mVertices;
};

// Intersection functions
bool Intersect(const Sphere3D& a, const Sphere3D& b);
bool Intersect(const AABB3D& a, const AABB3D& b);
bool Intersect(const Capsule3D& a, const Capsule3D& b);
bool Intersect(const Sphere3D& s, const AABB3D& box);

bool Intersect(const LineSegment3D& l, const Sphere3D& s, float& outT);
bool Intersect(const LineSegment3D& l, const Plane3D& p, float& outT);
bool Intersect(const LineSegment3D& l, const AABB3D& b, float& outT,
	Vector3& outNorm);

bool SweptSphere(const Sphere3D& P0, const Sphere3D& P1,
	const Sphere3D& Q0, const Sphere3D& Q1, float& t);
