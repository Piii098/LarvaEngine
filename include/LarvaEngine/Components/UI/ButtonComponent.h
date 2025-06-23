#pragma once
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Utilities/DataTypes.h"
#include <functional>
#include <string>

class SpriteComponent;
class TextComponent;
class Box2DComponent;
class InputAction;

class ButtonComponent : public Component {
public:
    enum class BUTTON_STATE {
        NORMAL,
        PRESSED,
        DISABLED,
		SELECTED
    };

    ButtonComponent(GameObject& parent, int updateLayer = 100);
    virtual ~ButtonComponent();

    // ���͏����̃I�[�o�[���C�h
    void ProcessInput(const InputAction& input) override;
    // �X�V�����̃I�[�o�[���C�h
    void Update(float deltaTime) override;

    void OnClick();
    
    // �N���b�N���̃R�[���o�b�N�ݒ�
    void SetOnClickCallback(std::function<void()> callback) { _onClickCallback = callback; }
    // �z�o�[���̃R�[���o�b�N�ݒ�
    void SetOnSelectCallback(std::function<void()> callback) { _onSelectCallback = callback; }

	void SetPressKey(const std::string& selectInputAction) { _selectInputAction = selectInputAction; }

    // �{�^���̗L��/������ݒ�
    void SetEnabled(bool enabled);
    bool IsEnabled() const { return _state == BUTTON_STATE::NORMAL || _state == BUTTON_STATE::PRESSED; }

    void SetSelected(bool selected);
	bool IsSelected() const { return _state == BUTTON_STATE::SELECTED; }

	bool IsHovered() const { return _isHovered; }

    // �{�^���̏�Ԃɉ����������ڂ̕ύX
    void SetNormalColor(const Vector3& color) { _normalColor = color; UpdateAppearance(); }
    void SetPressedColor(const Vector3& color) { _pressedColor = color; UpdateAppearance(); }
    void SetDisabledColor(const Vector3& color) { _disabledColor = color; UpdateAppearance(); }
	void SetSelectedColor(const Vector3& color) { _selectedColor = color; UpdateAppearance(); }

private:
    // �{�^���̏�Ԃ��X�V
    void UpdateAppearance();
    // �}�E�X���W���{�^�����ɂ��邩����
    bool IsPointInButton(float x, float y) const;

    BUTTON_STATE _state;
    std::function<void()> _onClickCallback;
	std::function<void()> _onSelectCallback;

    SpriteComponent* _spriteComponent;
    TextComponent* _textComponent;
	Box2DComponent* _boxComponent;

    Vector3 _normalColor;
    Vector3 _pressedColor;
    Vector3 _disabledColor;
	Vector3 _selectedColor;

    std::string _selectInputAction;

    bool _isPressed;
	bool _isHovered;
    bool _isSelected;
};