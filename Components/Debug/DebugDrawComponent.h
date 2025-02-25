#pragma once
#include "Components/SpriteComponent.h"

class BoxComponent2D;

/*
	ボックス状のスプライトを表示する
*/

class DebugDrawComponent : public SpriteComponent {
public:

	DebugDrawComponent(GameObject* parent,BoxComponent2D* boxComp, int drawLayer = 100);
	~DebugDrawComponent() override;

	void SetBox(const Vector2& position, const Vector2& size, const Vector3& color);
	void DrawBox(const Vector2& position, const Vector2& size, const Vector3& color);
	void Draw(Shader* shader) override;

private:
	BoxComponent2D* _boxComp;

	int _boxHeight;
	int _boxwidth;

};