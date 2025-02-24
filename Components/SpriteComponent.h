#pragma once
#include <unordered_map>
#include <string>
#include <SDL3/SDL.h>
#include  "Components/TextureComponent.h"


class GameObject;
class Shader;
class Texture;

/*
���O�Ƀ��[�h���ꂽ�X�v���C�g���Z�b�g���A
*/

class SpriteComponent : public TextureComponent {
public:

	SpriteComponent(GameObject* parent, int drawLayer = 150);
	~SpriteComponent();

	virtual void Draw(Shader* shader) override;

	/*�Q�b�^�[�Z�b�^�[*/

private:


};