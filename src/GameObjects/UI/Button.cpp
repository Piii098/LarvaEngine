#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Input/InputAction.h"

#pragma region コンストラクタ

Button::Button(Scene& scene, const Vector2& offset)
	: GameObject(scene)
	, _hoveredIndex(-1)
	, _offset(Vector2::Zero){

}

Button::~Button() {

}

#pragma endregion

#pragma region パブリック関数

void Button::InputObject(const InputAction& input) {
   
	HandleGamepadInput(input);
	HandleKeyboardInput(input);
	//HandleMouseInput(input);

}

void Button::UpdateObject(float deltaTime) {
	for (int i = 0; i < _buttonData.size(); i++) {
		if (i == _hoveredIndex) {
			_buttonData[i].textComp->Color(Vector3(0.8, 0.8, 0.8));
		}
		else {
			_buttonData[i].textComp->Color(Vector3(1, 1, 1));
		}
	}
}

bool Button::SetText(int index, const std::string& text, const std::string& fontName
	, int pointSize, const Vector3& color) {
	if (index < 0 || index >= _buttonData.size()) return false;

	if (_buttonData[index].textComp) {
		delete _buttonData[index].textComp;
	}

	_buttonData[index].textComp = &CreateComponent<TextComponent>(0, fontName);
	_buttonData[index].textComp->CreateTextTexture(text, color, pointSize);
	_buttonData[index].textComp->PositionOffset(index * _offset);
	UpdateButtonRects();
	return true;
}

void Button::SetOnClick(int index, std::function<void()> onClick) {
	if (index > _buttonData.size() - 1) return;
	_buttonData[index].onClick = onClick;
}

void Button::SetOffset(const Vector2& offset) {
	_offset = offset;

	for (int i = 0; i < _buttonData.size(); i++) {
		_buttonData[i].offset = offset * i;
		if (_buttonData[i].textComp) {
			_buttonData[i].textComp->PositionOffset(_buttonData[i].offset);
		}
		if (_buttonData[i].spriteComp) {
			_buttonData[i].spriteComp->PositionOffset(_buttonData[i].offset);
		}
	}


	UpdateButtonRects();
}

int Button::CreateButtonData(const std::string& text, const std::string& fontName,
	int pointSize, const Vector3& color,
	std::function<void()> onClick) {
	ButtonData data;

	// テキストが指定されていればテキストコンポーネントを作成
	if (!text.empty() && !fontName.empty()) {
		data.textComp = &CreateComponent<TextComponent>(0, fontName);
		data.textComp->CreateTextTexture(text, color, pointSize);
		data.textComp->PositionOffset(_buttonData.size() * _offset);
	}

	// クリック処理関数の設定
	data.onClick = onClick;

	_buttonData.push_back(data);

	UpdateButtonRects();

	return _buttonData.size() - 1;  // 追加したボタンのインデックスを返す
}

void Button::SelectButton(int index) {
	if (index < 0 || index >= _buttonData.size()) return;
	_hoveredIndex = index;
}

void Button::UpdateButtonRects() {
	Vector2 basePos = Vector2::ToFloat(Position());

	// 各ボタンの表示領域を更新
	for (size_t i = 0; i < _buttonData.size(); i++) {
		ButtonData& data = _buttonData[i];
		Vector2 pos = basePos + data.offset;
		SDL_Log("pos: %f, %f", pos.x, pos.y);
		int width = 0, height = 0;

		// テキストコンポーネントからサイズを取得
		if (data.textComp) {
			width = data.textComp->TexWidth();
			height = data.textComp->TexHeight();  // 修正：正しく高さを取得
		}

		// スプライトコンポーネントからサイズを取得（より大きい方を採用）
		if (data.spriteComp) {
			width = std::max(width, data.spriteComp->TexWidth());
			height = std::max(height, data.spriteComp->TexHeight());
		}

		// 中心座標からボタンの左上座標を計算
		pos.x = pos.x - width / 2;
		pos.y = pos.y - height / 2;

		// 表示領域を更新
		data.rect = {
			static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			width,
			height
		};
	}
}

#pragma endregion

#pragma region プライベート関数

void Button::HandleGamepadInput(const InputAction& input) {

}

void Button::HandleKeyboardInput(const InputAction& input) {
	if (_hoveredIndex == -1) {
		if (input.IsKeyDown(SDL_SCANCODE_DOWN) || input.IsKeyDown(SDL_SCANCODE_TAB)) {
			_hoveredIndex = 0;
		}
		return;
	}

	// 選択とナビゲーション
	if (input.IsKeyDown(SDL_SCANCODE_RETURN) || input.IsKeyDown(SDL_SCANCODE_SPACE) ||
		input.IsKeyDown(SDL_SCANCODE_D)) {
		if (_buttonData[_hoveredIndex].onClick) {
			_buttonData[_hoveredIndex].onClick();
		}
	}

	// 上下移動
	if (input.IsKeyDown(SDL_SCANCODE_UP)) {
		_hoveredIndex = (_hoveredIndex > 0) ? (_hoveredIndex - 1) : (_buttonData.size() - 1);
	}
	else if (input.IsKeyDown(SDL_SCANCODE_DOWN) || input.IsKeyDown(SDL_SCANCODE_TAB)) {
		_hoveredIndex = (_hoveredIndex < _buttonData.size() - 1) ? (_hoveredIndex + 1) : 0;
	}
}

bool IsPointInRect(const Vector2& point, const SDL_Rect& rect) {
	return (point.x >= rect.x && point.x < rect.x + rect.w &&
		point.y >= rect.y && point.y < rect.y + rect.h);
}

void Button::HandleMouseInput(const InputAction& input) {
	Vector2 mousePos = input.GetMousePosition();
	//SDL_Log("mousePos: %f, %f", mousePos.x, mousePos.y);
	// 各ボタンについてマウス位置チェック
	bool foundHover = false;
	for (size_t i = 0; i < _buttonData.size(); i++) {
		// 重要な変更：事前計算された表示領域を使用
		const SDL_Rect& buttonRect = _buttonData[i].rect;
		//SDL_Log("buttonRect: %d, %d, %d, %d", buttonRect.x, buttonRect.y, buttonRect.w, buttonRect.h);
		bool isHovered = IsPointInRect(mousePos, buttonRect);

		_buttonData[i].isHovered = isHovered;
		if (isHovered) {
			_hoveredIndex = i;
			foundHover = true;

			if (input.IsMouseButtonDown(GameTypes::SDL_MouseButton::SDL_MOUSE_LEFT)) {
				if (_buttonData[i].onClick) {
					_buttonData[i].onClick();
				}
			}
		}
	}

	if (!foundHover && _hoveredIndex != -1) {
		// マウスがどのボタンの上にもない場合はホバー状態を解除
		_hoveredIndex = -1;
	}
}

SDL_Rect Button::CalculateButtonRect(int index) const {
	if (index < 0 || index >= _buttonData.size()) {
		return { 0, 0, 0, 0 };
	}

	// 事前計算された表示領域を返す
	return _buttonData[index].rect;
}

#pragma endregion
