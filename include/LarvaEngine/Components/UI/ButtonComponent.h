#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include <functional>
#include <string>

class SpriteComponent;
class TextComponent;
class InputAction;

class ButtonComponent : public Component {
public:
    enum class BUTTON_STATE {
        NORMAL,
        HOVER,
        PRESSED,
        DISABLED
    };

    ButtonComponent(GameObject& parent, int updateLayer = 100);
    virtual ~ButtonComponent();

    // 入力処理のオーバーライド
    void ProcessInput(const InputAction& input) override;
    // 更新処理のオーバーライド
    void Update(float deltaTime) override;

    // クリック時のコールバック設定
    void SetOnClickCallback(std::function<void()> callback) { _onClickCallback = callback; }
    // ホバー時のコールバック設定
    void SetOnHoverCallback(std::function<void()> callback) { _onHoverCallback = callback; }

    // ボタンの有効/無効を設定
    void SetEnabled(bool enabled);
    bool IsEnabled() const { return _state == BUTTON_STATE::NORMAL || _state == BUTTON_STATE::HOVER || _state == BUTTON_STATE::PRESSED; }

    // ボタンの状態に応じた見た目の変更
    void SetNormalColor(const Vector3& color) { _normalColor = color; UpdateAppearance(); }
    void SetHoverColor(const Vector3& color) { _hoverColor = color; UpdateAppearance(); }
    void SetPressedColor(const Vector3& color) { _pressedColor = color; UpdateAppearance(); }
    void SetDisabledColor(const Vector3& color) { _disabledColor = color; UpdateAppearance(); }

private:
    // ボタンの状態を更新
    void UpdateAppearance();
    // マウス座標がボタン内にあるか判定
    bool IsPointInButton(float x, float y) const;

    BUTTON_STATE _state;
    std::function<void()> _onClickCallback;
    std::function<void()> _onHoverCallback;

    SpriteComponent* _spriteComponent;
    TextComponent* _textComponent;

    Vector3 _normalColor;
    Vector3 _hoverColor;
    Vector3 _pressedColor;
    Vector3 _disabledColor;

    bool _isPressed;
    bool _isHovered;
};