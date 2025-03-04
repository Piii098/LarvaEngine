#pragma once
#include <vector>
#include <functional>
#include "Utilities/Collision2D.h"
#include "Math.h"

class Frame;
class Game;
class GameObject;
class DynamicObject;
class StaticObject;
class BoxComponent2D;
class RigidbodyComponent;

class PhysWorld2D {
public:
    struct CollisionInfo {
        Vector2 _point;
        Vector2Int _normal;
        BoxComponent2D* _box;
        GameObject* _object;
    };

    PhysWorld2D(Game* game);

    // セグメントキャストを行い、衝突情報を取得する
    bool SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, const GameObject* ignoreObj = nullptr);

    void Update(float deltaTime); // フレームごとの更新処理
    void CollisionUpdate(float deltaTime); // 衝突の更新処理
    
	void CalculateVelocity(float deltaTime); // 速度を計算
    void UpdatePosition(float deltaTime);


    void AddRigidbodyComp(RigidbodyComponent* obj); // 動的コンポーネントを追加
    void AddBoxComp(BoxComponent2D* obj); // 静的ボックスコンポーネントを追加

    void RemoveRigidbodyComp(RigidbodyComponent* obj); // 動的コンポーネントを削除
    void RemoveBoxComp(BoxComponent2D* obj); // 静的ボックスコンポーネントを削除

private:
    // 衝突を修正する
    void FixCollision(BoxComponent2D* boxA, Vector2& velocityA, Vector2& nextPositionA, AABB2D& nextBoxA, BoxComponent2D* boxB);

	const float CCD_STEP_SIZE = 0.01f;


    Game* game;
    std::vector<BoxComponent2D*> _boxComps;
    std::vector<RigidbodyComponent*> _rigidbodyComps;
};