#include "LarvaEngine/GameObjects/UI/Button.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Components/Draw/TextComponent.h"
#include "LarvaEngine/Components/UI/ButtonComponent.h"

// ==============================================================================
// コンストラクタ・デストラクタ
// ==============================================================================

Button::Button(Scene& scene, const std::string& spriteName)
	: GameObject(scene)
{
	SpriteComponent& spri = CreateComponent<SpriteComponent>(10);
	spri.SetTexture(spriteName);
	TextComponent& text = CreateComponent<TextComponent>(10, "MOBO");

	ButtonComponent& button = CreateComponent<ButtonComponent>();

}

Button::~Button() {
	
}

// =============================================================================
// パブリック関数
// =============================================================================


void Button::SetOnClick(std::function<void()> callback) {
	GetComponent<ButtonComponent>()->SetOnClickCallback(callback);
}

void Button::SetOnHover(std::function<void()> callback) {
	GetComponent<ButtonComponent>()->SetOnHoverCallback(callback);
}
