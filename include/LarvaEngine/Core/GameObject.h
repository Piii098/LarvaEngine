#pragma once
#include <vector>
#include "LarvaEngine/Core/Utilities/Math.h"

class Input;
class Frame;
class Component;
class Game;
class ChildObject;
class GameScene;
class Scene;
class Shader;
class MainScene;

class GameObject {
public:

    enum class STATE {
        ACTIVE,
        INACTIVE,
        DEAD
    };

    enum class TAG {
        NONE,
        PLAYER,
        ENEMY,
        ITEM,
        GROUND,
        OBSTACLE
    };

    GameObject(Scene* scene);
    virtual ~GameObject();

    /*入力*/

    void ProcessInput(Input* input);
    virtual void InputObject(Input* input);

    /*更新*/

    void Update(float deltaTime); // Gameクラスで実行される更新
    void UpdateComponents(float deltaTime); // 全Componentの更新
    virtual void UpdateObject(float deltaTime); // 継承先オブジェクトのユニークな更新
    void PhysUpdate(float deltaTime);
	virtual void PhysUpdateComponents(float deltaTime);
	virtual void PhysUpdateObject(float deltaTime);

    void ComputeWorldTransform();

    /*コンポーネント処理*/

    void AddComponent(Component* component); //コンポーネントをつける(順序に合わせ)
    void RemoveComponent(Component* component);  //コンポーネントを取り外す

    void AddChildren(GameObject* childObject); //コンポーネントをつける(順序に合わせ)
    void RemoveChildren(GameObject* childObject);  //コンポーネントを取り外す


    template <typename T> T* GetComponent() const {

        for (auto comp : _components) {
            T* t = dynamic_cast<T*>(comp);
            if (t != nullptr) {
                return t;
            }
        }

        return nullptr;
    }

    /*ゲッターセッター*/

    Scene* GetScene() { return _scene; };
    MainScene* GetMainScene();
	GameObject* GetParent() { return _parent; };
	std::vector<GameObject*> GetChildren() { return _childrenObjects; };
	void SetParent(GameObject* parent) { _parent = parent; };
    
    Vector2 GetForward() const { return Vector2(Math::Cos(_rotation), Math::Sin(_rotation)); };

    const Matrix4& WorldTransform() const { return _worldTransform; };
    TAG Tag() const { return _tag; };
    STATE State() const { return _state; };
    float Scale() const { return _scale; };
    float Rotation() const { return _rotation; };
    void Position(const Vector2& internalPosition) { _position = Vector2Int::ToInterger(internalPosition); _recomputeWorldTransform = true; }
    void Position(const Vector2Int& position) { _position = position; _recomputeWorldTransform = true; }
    const Vector2Int& Position() const { return _position; };
    Vector2 PositionToFloat() const { return Vector2::ToFloat(_position); };
    void Scale(float scale) { _scale = scale; _recomputeWorldTransform = true; };
    void Rotation(float rotation) { _rotation = rotation; _recomputeWorldTransform = true; };
    void Tag(TAG tag) { _tag = tag; };
    void State(STATE state) { _state = state; };

private:

	Scene* _scene;

    STATE _state;
    TAG _tag;
    Game* _game;
    GameObject* _parent;

    /*トランスフォーム変数*/

    Matrix4 _worldTransform;
    bool _recomputeWorldTransform;

    Vector2Int _position; // 位置
    float _scale; // スケール
    float _rotation; // 回転

    std::vector<Component*> _components; //所持しているコンポーネントの集まり
	std::vector<GameObject*> _childrenObjects; //子オブジェクトの集まり
};