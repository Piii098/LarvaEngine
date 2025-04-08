#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/GameObject.h"

class SpriteComponent;
class TextComponent;
class InputAction;

struct ButtonData {
	bool isPressed = false;
	bool isHovered = false;
	std::function<void()> onClick = nullptr;
	SpriteComponent* spriteComp = nullptr;
	TextComponent* textComp = nullptr;
	SDL_Rect rect;      // ボタンの表示領域（スクリーン座標）
	Vector2 offset;     // 親オブジェクトからの相対位置
};

class Button : public GameObject {
public:
	Button(Scene& scene, const Vector2& offset = Vector2::Zero);
	~Button() override;

	void InputObject(const InputAction& input) override;

	void UpdateObject(float deltaTime) override;
	bool SetText(int index, const std::string& text, const std::string& fontName
		, int pointSize, const Vector3& color);

	void SetOnClick(int key, std::function<void()> onClick);
	
	int CreateButtonData(const std::string& text = "", const std::string& fontName = "",
		int pointSize = 16, const Vector3& color = Vector3(1, 1, 1)
		, std::function<void()> onClick = nullptr);

	void SetOffset(const Vector2& offset);

	void SelectButton(int index);

	int GetSelectedButton() const { return _hoveredIndex; }

	void UpdateButtonRects();  // ボタンの表示領域を更新

private:

	void HandleKeyboardInput(const InputAction& input);
	void HandleMouseInput(const InputAction& input);
	void HandleGamepadInput(const InputAction& input);

	SDL_Rect CalculateButtonRect(int index) const;

	Vector2 _offset;
	std::vector<ButtonData> _buttonData;
	int _hoveredIndex;

};