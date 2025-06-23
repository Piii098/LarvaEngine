#pragma once
#include <vector>
#include <functional>
#include "LarvaEngine/Physics/Collision2D.h"
#include "LarvaEngine/Core/Utilities/Math.h"

// ===== 前方宣言 ===== //
class Game;
class GameObject;
class Box2DComponent;
class Rigidbody2DComponent;

// 衝突情報
struct CollisionInfo {
	Vector2 _point;         ///< 衝突点
	Vector2 _normal;     ///< 衝突法線
	Box2DComponent* _box;   ///< 衝突したボックスコンポーネント
	GameObject* _object;    ///< 衝突したオブジェクト
};

/// @brief 2Dの物理世界クラス
///
/// 2Dの物理世界を管理する
/// 静的なボックスコンポーネントと動的なリジッドボディコンポーネントを管理する
/// 衝突判定、衝突修正を行う
class PhysWorld2D {
public:


	// ===== コンストラクタ =====//

    PhysWorld2D(Game& game);


	// ===== 衝突判定 =====//
    
	/// @brief 線分との衝突判定
	/// レイキャストとして使用
	/// @param l 線分
	/// @param outColl 衝突情報
	/// @param ignoreObj 無視するオブジェクト(自身のオブジェクト)
	/// @return 衝突したかどうか
    bool SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, GameObject& ignoreObj);


	// ===== 更新処理 =====//

	/// @brief 更新処理
	/// Game::FixedUpdate()から呼び出す
	/// @param deltaTime 補正されたデルタ時間
    void FixedUpdate(float deltaTime);

	// ===== ボックスコンポーネント関連 =====//

	/// @brief ボックスコンポーネントを追加
	/// @param obj ボックスコンポーネント
	void AddBoxComponent(Box2DComponent* obj); // 静的ボックスコンポーネントを追加
    
	/// @brief ボックスコンポーネントを削除
	/// @param obj ボックスコンポーネント
	void RemoveBoxComponent(Box2DComponent* obj); // 静的ボックスコンポーネントを削除

	/// @brief リジットボディコンポーネントを追加
	/// @param obj リジットボディコンポーネント
	void AddRigidbodyComponent(Rigidbody2DComponent* obj); // 動的リジッドボディコンポーネントを追加
	
	/// @brief リジットボディコンポーネントを追加
	/// @param obj リジットボディコンポーネント
	void RemoveRigidbodyComponent(Rigidbody2DComponent* obj); // 動的リジッドボディコンポーネントを削除

private:

	// ===== 衝突判定 =====//

	/// @brief 衝突判定
	/// サブステップでの衝突判定を行う
	/// @param deltaTime 補正されたデルタ時間
	void CollisionUpdate(float deltaTime);

	/// @brief 衝突修正
	/// 衝突したボックスコンポーネントを修正する
	/// @param boxA ボックスコンポーネントA
	/// @param velocityA 移動速度A
	/// @param nextPositionA 次の位置A
	/// @param nextBoxA 次のボックスA
    void FixCollision(Box2DComponent* boxA, Vector2& velocityA, Vector2& nextPositionA, AABB2D& nextBoxA, Box2DComponent* boxB);

	Game& game;								///< ゲームクラス
	const float CCD_STEP_SIZE = 0.01f; 		///< CCDのステップサイズ
	std::vector<Box2DComponent*> _boxComps; ///< ボックスコンポーネントの配列
	std::vector<Rigidbody2DComponent*> _rigidbodyComps; ///< リジッドボディコンポーネントの配列
};