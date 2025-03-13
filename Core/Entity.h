#pragma once
#include <vector>
#include "Utilities/Math.h"

class Component;
class Scene;
class Shader;
class Input;

class Entity {
public:
    enum class STATE {
        ACTIVE,
        PAUSED,
        DEAD
    };

    Entity(Scene* scene);
    virtual ~Entity();

    virtual void ProcessInput(Input* input);
    void InputComponents(Input* input);
    virtual void InputEntity(Input* input) {};

    virtual void Update(float deltaTime);
    void UpdateComponents(float deltaTime);
    virtual void UpdateEntity(float deltaTime) {};

    virtual void Render(Shader* shdaer);
    void RenderComponents(Shader* shader);

    void ComputeWorldTransform();

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);

    template <typename T> T* GetComponent() const {
        for (auto comp : _components) {
            T* t = dynamic_cast<T*>(comp);
            if (t != nullptr) {
                return t;
            }
        }
        return nullptr;
    }

    STATE State() const { return _state; }
    void State(STATE state) { _state = state; }
    void Position(const Vector2& internalPosition) { _position = Vector2Int::ToInterger(internalPosition); _recomputeWorldTransform = true; }
    void Position(const Vector2Int& position) { _position = position; _recomputeWorldTransform = true; }
    const Vector2Int& Position() const { return _position; };
    Vector2 PositionToFloat() const { return Vector2::ToFloat(_position); };
    float Scale() const { return _scale; };
    void Scale(float scale) { _scale = scale; _recomputeWorldTransform = true; };
    void Rotation(float rotation) { _rotation = rotation; _recomputeWorldTransform = true; };

    const Matrix4& WorldTransform() const { return _worldTransform; };

    Scene* GetScene() const { return _scene; };

    // êVÇµÇ¢âºëzä÷êîÇí«â¡
    virtual GameObject* AsGameObject() { return nullptr; };

protected:
    Vector2Int _position;
    float _scale;
    float _rotation;
    bool _recomputeWorldTransform;

    Matrix4 _worldTransform;

    Scene* _scene;
    STATE _state;
    std::vector<Component*> _components;
};