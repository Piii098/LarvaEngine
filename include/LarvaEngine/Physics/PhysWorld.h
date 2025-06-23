#pragma once
#include <vector>

class BoxComponent; // 前方宣言
class RigidbodyComponent; // 前方宣言
class Game; // 前方宣言

class PhysWorld {
public:
	PhysWorld(Game& game);
	virtual ~PhysWorld();

	virtual void FixedUpdate(float deltaTime);

	void AddBoxComponent(BoxComponent* obj); // 静的ボックスコンポーネントを追加
	void RemoveBoxComponent(BoxComponent* obj); // 静的ボックスコンポーネントを削除

	void AddRigidbodyComponent(RigidbodyComponent* obj); // 動的リジッドボディコンポーネントを追加
	void RemoveRigidbodyComponent(RigidbodyComponent* obj); // 動的リジッドボディコンポーネントを削除

protected:

	Game& _game; ///< ゲームクラス

	std::vector<BoxComponent*> _boxComps; ///< ボックスコンポーネントの配列
	std::vector<RigidbodyComponent*> _rigidbodyComps; ///< リジッドボディコンポーネントの配列

};