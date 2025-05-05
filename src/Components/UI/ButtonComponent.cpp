#include "LarvaEngine/Components/UI/ButtonComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Input/InputAction.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Components/Physics/BoxComponent2D.h"
#include "LarvaEngine/Physics/Collision2D.h"
#include <SDL3/SDL.h>


ButtonComponent::ButtonComponent(GameObject& parent, int updateLayer)
    : Component(parent, updateLayer)
    , _state(BUTTON_STATE::NORMAL)
    , _spriteComponent(nullptr)
    , _textComponent(nullptr)
    , _normalColor(Vector3(1.0f, 1.0f, 1.0f))
    , _pressedColor(Vector3(0.7f, 0.7f, 0.7f))
    , _disabledColor(Vector3(0.5f, 0.5f, 0.5f))
	, _selectedColor(Vector3(0.8f, 0.8f, 0.8f))
    , _isPressed(false)
    , _isSelected(false)
    , _selectInputAction("Select")
{
    // SpriteComponent��TextComponent���擾
    _spriteComponent = _parent.GetComponent<SpriteComponent>();
	_boxComponent = _parent.GetComponent<BoxComponent2D>();
	AABB2D myBox = _spriteComponent->GetAABB();
	_boxComponent->SetObjectBox(myBox);
    _textComponent = _parent.GetComponent<TextComponent>();
}

ButtonComponent::~ButtonComponent() {
}

void ButtonComponent::ProcessInput(const InputAction& input) {

    if (_state == BUTTON_STATE::DISABLED) {
        return;
    }

    bool wasPressed = _isPressed;
    bool wasSelected = _isSelected;

    // �}�E�X���{�^����ɂ��邩����
    _isHovered = IsPointInButton(input.GetMousePosition().x, input.GetMousePosition().y);

    // �}�E�X�{�^����������Ă��邩����
    _isPressed = _isHovered && input.IsMouseButtonDown(GameTypes::SDL_MouseButton::SDL_MOUSE_LEFT);

    // �z�o�[��Ԃɕω����������Ƃ��̏���
    if (_isSelected && !wasSelected) {
        _state = BUTTON_STATE::SELECTED;
        if (_onSelectCallback) {
            _onSelectCallback();
        }
    } else if (!_isSelected && wasSelected) {
        _state = BUTTON_STATE::NORMAL;
    }

    if (_isPressed && !wasPressed) {
        _state = BUTTON_STATE::PRESSED;
    }

	if (_state == BUTTON_STATE::SELECTED && input.IsActionDown(_selectInputAction)) {
		_state = BUTTON_STATE::PRESSED;
	}

    if (_state == BUTTON_STATE::PRESSED) {
        OnClick();
	}

    UpdateAppearance();
}

void ButtonComponent::Update(float deltaTime) {
    // ��Ԃɉ������X�V����
}

void ButtonComponent::OnClick() {
	if (_onClickCallback) {
		_onClickCallback();
	}
}

void ButtonComponent::SetEnabled(bool enabled) {
    _state = enabled ? BUTTON_STATE::NORMAL : BUTTON_STATE::DISABLED;
    UpdateAppearance();
}

void ButtonComponent::SetSelected(bool selected) {
	_state = selected ? BUTTON_STATE::SELECTED : BUTTON_STATE::NORMAL;
	UpdateAppearance();
}

void ButtonComponent::UpdateAppearance() {

    if (!_spriteComponent) return;

    Vector3 color;

    switch (_state) {
    case BUTTON_STATE::NORMAL:
        color = _normalColor;
        break;
    case BUTTON_STATE::PRESSED:
        color = _pressedColor;
        break;
    case BUTTON_STATE::DISABLED:
        color = _disabledColor;
        break;
	case BUTTON_STATE::SELECTED:
		color = _selectedColor;
        break;
    }

    _spriteComponent->Color(color);
}

Vector2 ConvertMouseToUIScene(float mouseX, float mouseY) {
    // �E�B���h�E�̕��ƍ������擾
    int windowWidth, windowHeight;
    SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &windowWidth, &windowHeight);

    // �E�B���h�E�̒��S���W
    float centerX = static_cast<float>(windowWidth) / 2.0f;
    float centerY = static_cast<float>(windowHeight) / 2.0f;

    // �}�E�X���W��UIScene���W�n�ɕϊ�
    float uiSceneX = mouseX - centerX;
    float uiSceneY = centerY - mouseY; // Y���͏㉺���]
    return Vector2(uiSceneX, uiSceneY);
}

bool ButtonComponent::IsPointInButton(float x, float y) const {
    
	if (!_boxComponent) return false;

    const AABB2D& box = _boxComponent->GetWorldBox();

	// �}�E�X���W��UIScene���W�n�ɕϊ�
	Vector2 mousePos = ConvertMouseToUIScene(x, y);

	// �{�^����AABB�ƃ}�E�X���W���r
	return box.Contains(Vector2(mousePos));
}