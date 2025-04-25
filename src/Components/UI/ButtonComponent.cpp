#include "LarvaEngine/Components/UI/ButtonComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Input/InputAction.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include <SDL3/SDL.h>

ButtonComponent::ButtonComponent(GameObject& parent, int updateLayer)
    : Component(parent, updateLayer)
    , _state(BUTTON_STATE::NORMAL)
    , _spriteComponent(nullptr)
    , _textComponent(nullptr)
    , _normalColor(Vector3(1.0f, 1.0f, 1.0f))
    , _hoverColor(Vector3(0.9f, 0.9f, 0.9f))
    , _pressedColor(Vector3(0.7f, 0.7f, 0.7f))
    , _disabledColor(Vector3(0.5f, 0.5f, 0.5f))
    , _isPressed(false)
    , _isHovered(false)
{
    // SpriteComponent��TextComponent���擾
    _spriteComponent = _parent.GetComponent<SpriteComponent>();
    _textComponent = _parent.GetComponent<TextComponent>();
}

ButtonComponent::~ButtonComponent() {
}

void ButtonComponent::ProcessInput(const InputAction& input) {

    if (_state == BUTTON_STATE::DISABLED) {
        return;
    }

    

    bool wasPressed = _isPressed;
    bool wasHovered = _isHovered;

    // �}�E�X���{�^����ɂ��邩����
    _isHovered = IsPointInButton(input.GetMousePosition().x, input.GetMousePosition().y);

    // �}�E�X�{�^����������Ă��邩����
    _isPressed = _isHovered && input.IsMouseButtonDown(GameTypes::SDL_MouseButton::SDL_MOUSE_LEFT);

	if (input.IsMouseButtonDown(GameTypes::SDL_MouseButton::SDL_MOUSE_LEFT)) {
		SDL_Log("Button Pressed,x: %f, y: %f", input.GetMousePosition().x, input.GetMousePosition().y);
	}

    // �z�o�[��Ԃɕω����������Ƃ��̏���
    if (_isHovered && !wasHovered) {
        _state = BUTTON_STATE::HOVER;
        if (_onHoverCallback) {
            _onHoverCallback();
        }
    }

    else if (!_isHovered && wasHovered) {
        _state = BUTTON_STATE::NORMAL;
    }

    // �����ꂽ��Ԃɕω����������Ƃ��̏���
    if (_isPressed && !wasPressed) {
        _state = BUTTON_STATE::PRESSED;
    }

    else if (!_isPressed && wasPressed && _isHovered) {
        // �{�^���̏�ŃN���b�N�𗣂����Ƃ�
        _state = BUTTON_STATE::HOVER;
        if (_onClickCallback) {
            _onClickCallback();
        }
    }

    UpdateAppearance();
}

void ButtonComponent::Update(float deltaTime) {
    // ��Ԃɉ������X�V����
}

void ButtonComponent::SetEnabled(bool enabled) {
    _state = enabled ? BUTTON_STATE::NORMAL : BUTTON_STATE::DISABLED;
    UpdateAppearance();
}

void ButtonComponent::UpdateAppearance() {

    if (!_spriteComponent) return;

    Vector3 color;

    switch (_state) {
    case BUTTON_STATE::NORMAL:
        color = _normalColor;
        break;
    case BUTTON_STATE::HOVER:
        color = _hoverColor;
        break;
    case BUTTON_STATE::PRESSED:
        color = _pressedColor;
        break;
    case BUTTON_STATE::DISABLED:
        color = _disabledColor;
        break;
    }
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

    if (!_spriteComponent) return false;

	Vector2 mousePos = ConvertMouseToUIScene(x, y);
    
    // SDL_Log("mousePos.x: %f, mousePos.y: %f", mousePos.x, mousePos.y);

    // �X�v���C�g�̈ʒu�ƃT�C�Y���画��
    Vector2Int position = _parent.Position();
    int texHeight = _spriteComponent->TexHeight();
	int texWidth = _spriteComponent->TexWidth();
    float scale = _parent.Scale();

    int width = static_cast<int>(texWidth * scale);
    int height = static_cast<int>(texHeight * scale);

    // ���S���ʒu�ɂȂ��Ă��邽�߁A������̍��W���v�Z
    int left = position.x - width / 2;
    int top = position.y - height / 2;
	//SDL_Log("ButtonComponent::IsPointInButton, left: %d, top: %d, width: %d, height: %d", left, top, width, height);
    
    return (x >= left && x <= left + width && y >= top && y <= top + height);
}