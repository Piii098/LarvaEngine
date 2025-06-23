#pragma once
#include "LarvaEngine/Components/Draw/SpriteComponent.h"

class Box2DComponent;

/*
	ボックス状のスプライトを表示する
*/

class DebugDrawComponent : public SpriteComponent {
public:

	DebugDrawComponent(GameObject& parent, Box2DComponent* boxComp);
	~DebugDrawComponent() override;

	void SetBox(const Vector2& position, const Vector2& size, const Vector3& color);
	void DrawBox(const Vector2& position, const Vector2& size, const Vector3& color);

private:
	Box2DComponent* _boxComp;

	int _boxHeight;
	int _boxwidth;

};