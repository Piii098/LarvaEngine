#pragma once
#include <vector>
#include <memory>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Component.h"

// ===== 前方宣言 ===== //
class FrameSystem;
class Component;
class Game;
class Scene;
class MainScene;
class InputAction;

///@brief ゲーム内のオブジェクトの基本クラス
///
///コンポーネントを持ち、入力処理、更新処理を行う
///子オブジェクトを持つことができる
///DEAD状態のオブジェクトは破棄される
class GameObject {
public:

    enum class STATE {
        ACTIVE,   ///< アクティブ状態
        INACTIVE, ///< 非アクティブ状態
        DEAD      ///< 破棄予定状態
    };

    /// @brief タグ
    /// 未使用
    enum class TAG {
        NONE,     ///< タグなし
        PLAYER,   ///< プレイヤー
        ENEMY,    ///< 敵
        ITEM,     ///< アイテム
        GROUND,   ///< 地面
        OBSTACLE  ///< 障害物
    };

    // ===== コンストラクタ・デストラクタ =====　//

    GameObject(Scene& scene);

    GameObject(GameObject* parent);

    virtual ~GameObject();

    // ===== ループ処理 ===== //

    /// @brief 入力処理
    /// コンポーネントと独自の入力処理を呼び出す
    /// シーンより呼び出される
    /// @param input 入力情報
    void ProcessInput(const InputAction& input);

    /// @brief 更新処理
    /// コンポーネントと独自の入力処理を呼び出す
    /// シーンより呼び出される
    /// @param deltaTime フレーム間の時間
    void Update(float deltaTime);

    /// @brief 物理更新処理
    /// コンポーネントと独自の入力処理を呼び出す
    /// シーンより呼び出される 
    /// @param deltaTime フレーム間の時間
    void FixedUpdate(float deltaTime);

    void LateUpdate(float deltaTime);

    void Cleanup();


    // ====== ワールド変換行列関連 ===== //

    /// @brief ワールド変換行列の計算
    /// 位置、スケール、回転からワールド変換行列を計算する
    void ComputeWorldTransform();


    // ===== コンポーネント関連 ===== //

    /// @brief コンポーネントの削除
    /// コンポーネントを削除する
    /// @param component 削除するコンポーネント 
    void RemoveComponent(Component* component);

	/// @brief コンポーネントの生成
	/// コンポーネントを生成し、追加する
	/// @tparam T 生成するコンポーネントの型
	/// @tparam Args コンストラクタに渡す引数の型
	/// @param args コンストラクタに渡す引数 ( 第一引数のGameObjectへの参照は自動で設定される )
	/// @return 生成したコンポーネントへの参照( 一時的な参照を推奨 )
    template<typename T, typename... Args>
    T& CreateComponent(Args&&... args) {
        auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
        T& compRef = *component; // 生ポインタではなく参照を取得
        _components.emplace_back(std::move(component));
        return compRef;
    }

    /// @brief コンポーネントの取得
    /// 指定した型のコンポーネントを取得する
    /// @tparam T コンポーネントの型
    /// @return コンポーネントのポインタ
    template <typename T>
    T* GetComponent() {
        for (auto& comp : _components) {
            if (T* t = dynamic_cast<T*>(comp.get())) { // unique_ptrから生ポインタを取得
                return t;
            }
        }
        return nullptr;
    }

    void DestroyComponent(Component* component);

    void Destroy();
    void AddChildObject(GameObject* child);
	void RemoveChildObject(GameObject* child);
	
    bool IsEqual(const GameObject& object) const { return this == &object; } ///< 自分と同じオブジェクトかどうかを判定する


    // ===== ゲッターセッター ===== //

    void Position(const Vector3& position) { _position = position; _recomputeWorldTransform = true; }
	void Position(float x, float y, float z) { _position = Vector3(x, y, z); _recomputeWorldTransform = true; }
	void Position(const Vector2& position) { _position = Vector3(round(position.x), round(position.y), 0); _recomputeWorldTransform = true; }
	void Position(const Vector2Int& position) { _position = Vector3(static_cast<float>(position.x), static_cast<float>(position.y), 0); _recomputeWorldTransform = true; }
    void Scale(float scale) { _scale = scale; _recomputeWorldTransform = true; }
    void Rotation(Quaternion rotation) { _rotation = rotation; _recomputeWorldTransform = true; }
    void Tag(TAG tag) { _tag = tag; }
    void State(STATE state) { _state = state; }
    void SetParent(GameObject* parent) { _parent = parent; };

	const Vector3& GetForward() const { return Vector3::Transform(Vector3::UnitZ, _rotation); } ///< 前方ベクトルを取得する
    const TAG Tag() const { return _tag; }
    const STATE State() const { return _state; }
    const float Scale() const { return _scale; }
    const Quaternion Rotation() const { return _rotation; }
    const Matrix4& WorldTransform() const { return _worldTransform; }
    const Vector3& Position() const { return _position; }
    const Vector3& Position()  { return _position; }
	const Vector2Int& Position2D() { return Vector2(_position.x, _position.y); }
	const Vector3& GetRight() const { return Vector3::Transform(Vector3::UnitX, _rotation); } ///< 右方向ベクトルを取得する
	const Vector3& GetForward() { return Vector3::Transform(Vector3::UnitY, _rotation); } ///< 前方ベクトルを取得する

    Scene& GetScene() { return _scene; }
    MainScene& GetMainScene();

private:

    // ===== ループ処理 ===== //

    /// @brief コンポーネントの入力処理
    /// アクティブ状態のコンポーネントの入力処理を行う
    /// @param input 入力情報
    void InputComponents(const InputAction& input);

    /// @brief オブジェクトの入力処理
    /// 仮想関数、オブジェクト独自の入力処理を行う
    /// @param input 
    virtual void InputObject(const InputAction& input) {};

    /// @brief コンポーネントの更新処理
    /// アクティブ状態のコンポーネントの更新処理を行う
    /// @param deltaTime フレーム間の時間
    void UpdateComponents(float deltaTime);

    /// @brief オブジェクトの更新処理
    /// 仮想関数、オブジェクト独自の更新処理を行う
    /// @param deltaTime フレーム間の時間
    virtual void UpdateObject(float deltaTime) {};

	/// @brief コンポーネントの物理更新処理
	/// アクティブ状態のコンポーネントの物理更新処理を行う
	/// @param deltaTime フレーム間の時間
    void FixedUpdateComponents(float deltaTime);

	/// @brief オブジェクトの物理更新処理
	/// 仮想関数、オブジェクト独自の物理更新処理を行う
	/// @param deltaTime フレーム間の時間
    virtual void FixedUpdateObject(float deltaTime) {};

    void LateUpdateConponents(float deltaTime);

    virtual void LateUpdateObject(float deltaTime) {};


	// ===== オブジェクト関連 ===== //

    /// @brief コンポーネントの追加
    /// UpdateLayerの値が小さい順に挿入する
    /// @param component 追加するコンポーネント
    void AddComponent(std::unique_ptr<Component> component);


	// ===== メンバ変数 ===== //
    
    // シーン関連
    Scene& _scene;                              ///< 所属するシーン
	GameObject* _parent;                        ///< 親オブジェクト

    // 状態関連
    STATE _state;                               ///< オブジェクトの状態
    TAG _tag;                                   ///< オブジェクトのタグ

    // コンポーネント
    std::vector <std::unique_ptr<Component>> _components;        ///< コンポーネントリスト
	std::vector <GameObject*> _children;          ///< 子オブジェクトリスト

    // トランスフォーム関連
    Matrix4 _worldTransform;                    ///< ワールド変換行列
    bool _recomputeWorldTransform;              ///< 変換行列の再計算フラグ
    Vector3 _position;                       ///< 位置（整数）
	Quaternion _rotation;                     ///< 回転
    float _scale;                               ///< スケール
 
};

#include "LarvaEngine/Core/GameObject.inl"