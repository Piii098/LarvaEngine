#pragma once
#include "Utilities/Math.h"
#include <string>

class Shader;
class Input;
class Texture;
class Game;
class Scene;
class Font;

class UIScreen {
public:
	enum class STATE {
		ACTIVE,
		CLOSING
	};

	UIScreen(Scene* scene);
	virtual ~UIScreen();

	virtual void ProcessInput(Input* input);
	virtual void HandleKeyPress(int key);
	virtual void Update(float deltaTime);
	virtual void Render(Shader* shader);

	void Close() { _state = STATE::CLOSING; }


	STATE State() const { return _state; };
	void SetTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);

	Scene* GetScene() const { return _scene; }

protected:

	void DrawTexture(Shader* shader, Texture* texture, const Vector2& offset = Vector2::Zero, float = 1.f);

	Scene* _scene;
	Font* _font;
	Texture* _title;
	Vector2 _titlePos;

	STATE _state;
};