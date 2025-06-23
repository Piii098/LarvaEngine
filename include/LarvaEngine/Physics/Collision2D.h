#pragma once
#include "LarvaEngine/Core/Utilities/Math.h"
#include <vector>

// 2D コリジョンの基本構造体

/// @brief 2Dの線分
struct LineSegment2D{

	// ===== コンストラクタ =====//
	
    /// @brief コンストラクタ
	/// @param start 線分の始点
	/// @param end 線分の終点
    LineSegment2D(const Vector2& start, const Vector2& end);


	// ===== メンバ関数 =====//

	/// @brief 線分上の点を求める
	/// @param t 線分の始点からの距離
	/// @return 線分上の点
    Vector2 PointOnSegment(float t) const;

	/// @brief 点と線分の最短距離の2乗を求める
	/// @param point 点
	/// @return 点と線分の最短距離の2乗
    float MinDistSq(const Vector2& point) const;

	/// @brief 2つの線分の最短距離の2乗を求める
	/// @param s 線分
	/// @return 2つの線分の最短距離の2乗
    float MinDistSq(const LineSegment2D& s) const;

	/// @brief 2つの線分の最短距離の2乗を求める
	/// @param s1 線分1
	/// @param s2 線分2
	/// @return 2つの線分の最短距離の2乗
    static float MinDistSq(const LineSegment2D& s1, const LineSegment2D& s2);

	Vector2 _start;  ///< 線分の始点
	Vector2 _end;    ///< 線分の終点
};


/// @brief 2Dの円
struct Circle{

	// ===== コンストラクタ =====//

	/// @brief コンストラクタ
	/// @param center 円の中心
	/// @param radius 円の半径
    Circle(const Vector2& center, float radius);


	// ===== メンバ関数 =====//

	/// @brief 点が円の内部にあるかどうかを判定する
	/// @param point 点
	/// @return 点が円の内部にあるかどうか
    bool Contains(const Vector2& point) const;

    Vector2 _center;
    float _radius;
};


/// @brief 2DのAABB
struct AABB2D{

	// ===== コンストラクタ =====//

	/// @brief コンストラクタ
	/// @param min AABBの最小値(左下)
	/// @param max AABBの最大値(右上)
    AABB2D(const Vector2& min, const Vector2& max);


	// ===== メンバ関数 =====//

	/// @brief AABBの最小値と最大値を更新する
	/// @param point 点
    void UpdateMinMax(const Vector2& point);

	/// @brief 点がAABBの内部にあるかどうかを判定する
	/// @param point 点
    bool Contains(const Vector2& point) const;

	/// @brief 点とAABBの最短距離の2乗を求める
	/// @param point 点
    float MinDistSq(const Vector2& point) const;

	/// @brief AABB同士の交差判定
	/// @param other 判定するAABB
    void MoveCenterTo(const Vector2& targetPoint);

	/// @brief AABBの中心を求める
	/// @return AABBの中心
    Vector2 Center() const;

	Vector2 _min;	///< AABBの最小値(左下)
	Vector2 _max;	///< AABBの最大値(右上)
};


/// @brief 2Dのカプセル
struct Capsule2D{

	// ===== コンストラクタ =====//

	/// @brief コンストラクタ
	/// @param start カプセルの始点
	/// @param end カプセルの終点
	/// @param radius カプセルの半径
    Capsule2D(const Vector2& start, const Vector2& end, float radius);

	// ===== メンバ関数 =====//

	/// @brief 線分上の点を求める
	/// @param t 線分の始点からの距離
	/// @return 線分上の点
    Vector2 PointOnSegment(float t) const;

	/// @brief 点がカプセルの内部にあるかどうかを判定する
	/// @param point 点
	/// @return 点がカプセルの内部にあるかどうか
    bool Contains(const Vector2& point) const;

	LineSegment2D _segment;		///< カプセルの線分
	float _radius;				///< カプセルの半径
};


/// @brief 2Dの凸多角形
struct ConvexPolygon2D{
	/// @brief 凸多角形の頂点を指定して初期化
	/// @param vertices 頂点の配列
    bool Contains(const Vector2& point) const;

	std::vector<Vector2> _vertices;	///< 頂点の配列
};


/// @brief 2Dの交差判定

bool Intersect(const Circle& a, const Circle& b);       ///< 円と円の交差判定
bool Intersect(const AABB2D& a, const AABB2D& b);       ///< AABBとAABBの交差判定
bool Intersect(const Capsule2D& a, const Capsule2D& b); ///< カプセルとカプセルの交差判定
bool Intersect(const Circle& c, const AABB2D& box);     ///< 円とAABBの交差判定

bool TestSidePlane(float start, float end, float negd, const Vector2& norm, std::vector<std::pair<float, Vector2>>& out); ///< 2D交差判定の補助関数
bool Intersect(const LineSegment2D& l, const AABB2D& b, float& outT, Vector2& outNorm); ///< 線分とAABBの交差判定
