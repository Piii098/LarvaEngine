#include <algorithm>
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Components/Physics/Box2DComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Components/Physics/Rigidbody2DComponent.h"


//============================================================
// コンストラクタ・デストラクタ
//============================================================

PhysWorld2D::PhysWorld2D(Game& game)
    : game(game) {
}


//============================================================
// パブリック関数
//============================================================


// ===== 衝突判定 =====//

/**
 * 線分との衝突判定
 * 
 * 
 */
bool PhysWorld2D::SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, GameObject& ignoreObj) {
    bool collided = false;
    float closestT = std::numeric_limits<float>::infinity();
    Vector2 norm;

    // 衝突判定のためのループ
    for (auto box : _boxComps) {
        float t;
        if (box->GetParent().IsEqual(ignoreObj)) continue; // 無視するオブジェクトをスキップ

        if (Intersect(l, box->GetWorldBox(), t, norm)) { // 衝突判定
            if (t < closestT) { // 最も近い衝突点を更新
                closestT = t;
                outColl._point = l.PointOnSegment(t);
                outColl._normal = norm;
                outColl._box = box;
				outColl._object = &box->GetParent();
                collided = true;
            }
        }
    }
    return collided; // 衝突したかどうかを返す
}

// ===== 更新処理 =====//

/**
 * 更新処理
 *
 * Game::FixedUpdate()から呼びだされる
 */
void PhysWorld2D::FixedUpdate(float deltaTime) {
    CollisionUpdate(deltaTime); // 衝突の更新

    for (auto& rigid : _rigidbodyComps) {
		rigid->CalculateVelocity(deltaTime); // 速度の計算
		rigid->UpdatePosition(deltaTime); // 位置の更新
    }
}

/**
 * ボックスコンポーネントを追加
 */
void PhysWorld2D::AddBoxComponent(Box2DComponent* obj) {
    _boxComps.push_back(obj); // ボックスコンポーネントを追加
}

/**
 * ボックスコンポーネントを削除
 */
void PhysWorld2D::RemoveBoxComponent(Box2DComponent* box) {
    auto iter = std::find(_boxComps.begin(), _boxComps.end(), box);
    if (iter != _boxComps.end()) {
        std::iter_swap(iter, _boxComps.end() - 1); // 最後の要素とスワップ
        _boxComps.pop_back(); // 最後の要素を削除
    }
}

/**
 * リジットボディコンポーネントを追加
 */
void PhysWorld2D::AddRigidbodyComponent(Rigidbody2DComponent* obj) {
	_rigidbodyComps.push_back(obj); // リジットボディコンポーネントを追加
}

/**
 * リジットボディコンポーネントを削除
 */
void PhysWorld2D::RemoveRigidbodyComponent(Rigidbody2DComponent* obj) {
	auto iter = std::find(_rigidbodyComps.begin(), _rigidbodyComps.end(), obj);
	if (iter != _rigidbodyComps.end()) {
		std::iter_swap(iter, _rigidbodyComps.end() - 1); // 最後の要素とスワップ
		_rigidbodyComps.pop_back(); // 最後の要素を削除
	}
}


//============================================================
// プライベート関数
//============================================================


// ===== 衝突判定 =====//

/**
 * 衝突判定
 *
 * サブステップでの衝突判定を行う
 *
 */
void PhysWorld2D::CollisionUpdate(float deltaTime) {
    int subSteps = 5; // サブステップの数
    float subDeltaTime = deltaTime / subSteps; // サブステップごとの時間

    for (int i = 0; i < subSteps; ++i) {
        for (auto boxA : _boxComps) {
            auto rigidA = boxA->GetParent().GetComponent<Rigidbody2DComponent>();
            if (rigidA == nullptr || !boxA->IsCollision() || !boxA->IsDynamic()) {
                continue; // 衝突判定が無効な場合はスキップ
            }

            Vector2 currentPosA = rigidA->InternalPosition(); // 現在の位置
            Vector2 velocityA = rigidA->Velocity(); // 現在の速度
            Vector2 nextPosA = currentPosA + velocityA * subDeltaTime; // 次の位置

            AABB2D nextBoxA = boxA->GetWorldBox();
            nextBoxA.MoveCenterTo(nextPosA); // 次のボックスの位置を更新

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


/**
 * 衝突修正
 *
 * 衝突したボックスコンポーネントを修正する
 *
 */
void PhysWorld2D::FixCollision(Box2DComponent* boxA, Vector2& velocityA, Vector2& nextPosA, AABB2D& nextBoxA, Box2DComponent* boxB) {
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
