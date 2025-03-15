#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>
#include  "Components/Draw/TextureComponent.h"


class GameObject;
class Shader;
class Texture;

/*
事前にロードされたスプライトをセットし、
*/

class SpriteComponent : public TextureComponent {
public:

	SpriteComponent(GameObject* parent, int bufferLayer, int drawLayer = 150);
	~SpriteComponent() override;

	/*ゲッターセッター*/

};