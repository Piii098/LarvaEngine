#include <algorithm>
#include "PhysWorld2D.h"
#include "Components/Phys/BoxComponent2D.h"
#include "Core/Game.h"
#include "Renderer/Renderer.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Frame.h"
#include "Components/Phys/RigidbodyComponent.h"

#pragma region コンストラクタ

PhysWorld2D::PhysWorld2D(Game* game)
    : game(game) {
}

#pragma endregion

#pragma region 衝突判定

bool PhysWorld2D::SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, const GameObject* ignoreObj) {
    bool collided = false;
    float closestT = std::numeric_limits<float>::infinity();
    Vector2Int norm;

    // 衝突判定のためのループ
    for (auto box : _boxComps) {
        float t;
        if (box->GetParent() == ignoreObj) continue; // 無視するオブジェクトをスキップ

        if (Intersect(l, box->GetWorldBox(), t, norm)) { // 衝突判定
            if (t < closestT) { // 最も近い衝突点を更新
                closestT = t;
                outColl._point = l.PointOnSegment(t);
                outColl._normal = norm;
                outColl._box = box;
                outColl._object = box->GetParent();
                collided = true;
            }
        }
    }
    return collided; // 衝突したかどうかを返す
}

void PhysWorld2D::Update(float deltaTime) {
    CalculateVelocity(deltaTime); // 速度の計算
    CollisionUpdate(deltaTime); // 衝突の更新
    UpdatePosition(deltaTime); // 位置の更新
}

void PhysWorld2D::RemoveRigidbodyComp(RigidbodyComponent* box) {
    auto iter = std::find(_rigidbodyComps.begin(), _rigidbodyComps.end(), box);
    if (iter != _rigidbodyComps.end()) {
        std::iter_swap(iter, _rigidbodyComps.end() - 1); // 最後の要素とスワップ
        _rigidbodyComps.pop_back(); // 最後の要素を削除
    }
}

void PhysWorld2D::RemoveBoxComp(BoxComponent2D* box) {
    auto iter = std::find(_boxComps.begin(), _boxComps.end(), box);
    if (iter != _boxComps.end()) {
        std::iter_swap(iter, _boxComps.end() - 1); // 最後の要素とスワップ
        _boxComps.pop_back(); // 最後の要素を削除
    }
}

void PhysWorld2D::AddRigidbodyComp(RigidbodyComponent* obj) {
    _rigidbodyComps.push_back(obj); // 剛体コンポーネントを追加
}

void PhysWorld2D::AddBoxComp(BoxComponent2D* obj) {
    _boxComps.push_back(obj); // ボックスコンポーネントを追加
}

#pragma endregion

#pragma region 物理計算

void PhysWorld2D::CalculateVelocity(float deltaTime) {
    for (auto rigid : _rigidbodyComps) {
        rigid->CalculateVelocity(deltaTime); // 各剛体の速度を計算
    }
}

void PhysWorld2D::UpdatePosition(float deltaTime) {
    for (auto rigid : _rigidbodyComps) {
        rigid->UpdatePosition(deltaTime); // 各剛体の位置を更新
    }
}

void PhysWorld2D::CollisionUpdate(float deltaTime) {
    int subSteps = 5; // サブステップの数
    float subDeltaTime = deltaTime / subSteps; // サブステップごとの時間

    for (int i = 0; i < subSteps; ++i) {
        for (auto boxA : _boxComps) {
            auto rigidA = boxA->GetParent()->GetComponent<RigidbodyComponent>();
            if (!rigidA || !boxA->IsCollision() || !boxA->IsDynamic()) {
                continue; // 衝突判定が無効な場合はスキップ
            }

            Vector2 currentPosA = rigidA->InternalPosition(); // 現在の位置
            Vector2 velocityA = rigidA->Velocity(); // 現在の速度
            Vector2 nextPosA = currentPosA + velocityA * subDeltaTime; // 次の位置

            AABB2D nextBoxA = boxA->GetWorldBox();
            nextBoxA.MoveCenterTo(Vector2Int::ToInterger(nextPosA)); // 次のボックスの位置を更新

            // 他のボックスとの衝突判定
            for (auto boxB : _boxComps) {
                if (boxA == boxB || !boxB->IsCollision()) continue; // 自分自身または無効なボックスをスキップ

                if (Intersect(nextBoxA, boxB->GetWorldBox())) {
                    FixCollision(boxA, velocityA, nextPosA, nextBoxA, boxB); // 衝突解決
                }
            }
            rigidA->Velocity(velocityA); // 速度を更新
            rigidA->InternalPosition(nextPosA); // 位置を更新
        }
    }
}

void PhysWorld2D::FixCollision(BoxComponent2D* boxA, Vector2& velocityA, Vector2& nextPosA, AABB2D& nextBoxA, BoxComponent2D* boxB) {
    AABB2D dynBox = nextBoxA; // 動的ボックス
    AABB2D staBox = boxB->GetWorldBox(); // 静的ボックス

    // 衝突解決のための移動量を計算
    float dx1 = staBox._min.x - dynBox._max.x;
    float dx2 = staBox._max.x - dynBox._min.x;
    float dy1 = staBox._min.y - dynBox._max.y;
    float dy2 = staBox._max.y - dynBox._min.y;

    // X方向かY方向の最小移動量を選択
    float dx = (std::abs(dx1) < std::abs(dx2)) ? dx1 : dx2;
    float dy = (std::abs(dy1) < std::abs(dy2)) ? dy1 : dy2;

    // 位置と速度の更新
    if (std::abs(dx) < std::abs(dy)) {
        nextPosA.x += dx; // X方向の位置を更新
        if (velocityA.x > 0 && dx < 0 || velocityA.x < 0 && dx > 0) {
            velocityA.x = 0.f; // 速度をゼロに
        }
    }
    else {
        nextPosA.y += dy; // Y方向の位置を更新
        if (velocityA.y > 0 && dy < 0 || velocityA.y < 0 && dy > 0) {
            velocityA.y = 0.f; // 速度をゼロに
        }
    }

    // ワールド変換の更新
    boxA->OnUpdateWorldTransform();
    boxB->OnUpdateWorldTransform();
}

#pragma endregion
