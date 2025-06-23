#pragma once
#include <vector>
#include <functional>
#include "LarvaEngine/Physics/Collision3D.h"
#include "LarvaEngine/Core/Utilities/Math.h"

// ===== 前方宣言 ===== //
class Game;
class GameObject;
class Box3DComponent;
class Rigidbody3DComponent;

// 衝突情報
struct CollisionInfo3D {
    Vector3 _point;         ///< 衝突点
    Vector3 _normal;        ///< 衝突法線
    Box3DComponent* _box;   ///< 衝突したボックスコンポーネント
    GameObject* _object;    ///< 衝突したオブジェクト
};

/// @brief 3Dの物理世界クラス
///
/// 3Dの物理世界を管理する
/// 静的なボックスコンポーネントと動的なリジッドボディコンポーネントを管理する
/// 衝突判定、衝突修正を行う
class PhysWorld3D {
public:
    // ===== コンストラクタ =====//
    PhysWorld3D(Game& game);

    // ===== 衝突判定 =====//
    /// @brief 線分との衝突判定
    /// レイキャストとして使用
    bool SegmentCast(const LineSegment3D& l, CollisionInfo3D& outColl, GameObject& ignoreObj);

    // ===== 更新処理 =====//
    /// @brief 更新処理
    void FixedUpdate(float deltaTime);

    // ===== ボックスコンポーネント関連 =====//
    void AddBoxComponent(Box3DComponent* obj);
    void RemoveBoxComponent(Box3DComponent* obj);
    void AddRigidbodyComponent(Rigidbody3DComponent* obj);
    void RemoveRigidbodyComponent(Rigidbody3DComponent* obj);

private:
    // ===== 衝突判定 =====//
    void CollisionUpdate(float deltaTime);
    void FixCollision(Box3DComponent* boxA, Vector3& velocityA, Vector3& nextPositionA, AABB3D& nextBoxA, Box3DComponent* boxB);

    Game& _game;                                ///< ゲームクラス
    const float CCD_STEP_SIZE = 0.01f;          ///< CCDのステップサイズ
    std::vector<Box3DComponent*> _boxComps;    ///< ボックスコンポーネントの配列
    std::vector<Rigidbody3DComponent*> _rigidbodyComps; ///< リジッドボディコンポーネントの配列
};