#include <fstream>
#include <string>
#include <sstream>
#include "Components/TileMapSpriteComponent.h"
#include "Utilities/Texture.h"
#include "Utilities/Shader.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "GameObjects/Camera.h"
#include "Components/TileMapComponent.h"

#pragma region �R���X�g���N�^�F�f�X�g���N�^

TileMapSpriteComponent::TileMapSpriteComponent(GameObject* parent, int drawLayer)
	: SpriteComponent(parent, drawLayer)
	, _tileMapComp(nullptr){

}

TileMapSpriteComponent::~TileMapSpriteComponent() {

}

#pragma endregion

#pragma region �p�u���b�N�֐�

void TileMapSpriteComponent::Draw(Shader* shader) {

	if (_texture) {

		int tilesPerRow = _texture->Width() / _tileMapComp->TileSize();

		for (int mapY = 0; mapY < _tileMapComp->MapHeight(); mapY++) {
			for (int mapX = 0; mapX < _tileMapComp->MapWidth(); mapX++) {

				int tileID = _tileMapComp->Tiles()[mapY][mapX];

				if (tileID == -1) continue; // -1�͋󔒃^�C���Ƃ��ĕ`�悵�Ȃ�

				// �^�C���̃��[���h���W���v�Z
				float dstX = mapX * _tileMapComp->TileSize();
				float dstY = (_tileMapComp->MapHeight() - 1 - mapY) * _tileMapComp->TileSize();

				// ���f���s����쐬
				Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(_tileMapComp->TileSize()), static_cast<float>(_tileMapComp->TileSize()), 1.0f);
				Matrix4 transMat = Matrix4::CreateTranslation(Vector3(dstX, dstY, 0.0f));
				Matrix4 world = scaleMat * transMat * _parent->WorldTransform();

				// �r���[�s����擾���A�ŏI�s����v�Z

				int tileRow = tileID / tilesPerRow;
				int tileCol = tileID % tilesPerRow;

				Vector2 texScale(_tileMapComp->TileSize() / static_cast<float>(_texture->Width()), _tileMapComp->TileSize() / static_cast<float>(_texture->Height()));

				Vector2 texOffset = Vector2(tileCol * texScale.x, tileRow * texScale.y); // �؂�o�������̍���

				// �V�F�[�_�[�ɍs���ݒ�
				shader->SetMatrixUniform("uWorldTransform", world);
				shader->SetVector2Uniform("uTexOffset", texOffset);
				shader->SetVector2Uniform("uTexScale", texScale);

				// �e�N�X�`�����A�N�e�B�u�ɂ��ĕ`��
				_texture->SetActive();
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			}
		}
	}

}

void TileMapSpriteComponent::SetTileMapComponent(TileMapComponent* tileMapComp) {
	_tileMapComp = tileMapComp;
}

#pragma endregion

#pragma region �v���C�x�[�g�֐�



#pragma endregion

