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

#pragma region コンストラクタ：デストラクタ

TileMapSpriteComponent::TileMapSpriteComponent(GameObject* parent, int drawLayer)
	: SpriteComponent(parent, drawLayer)
	, _tileMapComp(nullptr){

}

TileMapSpriteComponent::~TileMapSpriteComponent() {

}

#pragma endregion

#pragma region パブリック関数

void TileMapSpriteComponent::Draw(Shader* shader) {

	if (_texture) {

		int tilesPerRow = _texture->Width() / _tileMapComp->TileSize();

		for (int mapY = 0; mapY < _tileMapComp->MapHeight(); mapY++) {
			for (int mapX = 0; mapX < _tileMapComp->MapWidth(); mapX++) {

				int tileID = _tileMapComp->Tiles()[mapY][mapX];

				if (tileID == -1) continue; // -1は空白タイルとして描画しない

				// タイルのワールド座標を計算
				float dstX = mapX * _tileMapComp->TileSize();
				float dstY = (_tileMapComp->MapHeight() - 1 - mapY) * _tileMapComp->TileSize();

				// モデル行列を作成
				Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(_tileMapComp->TileSize()), static_cast<float>(_tileMapComp->TileSize()), 1.0f);
				Matrix4 transMat = Matrix4::CreateTranslation(Vector3(dstX, dstY, 0.0f));
				Matrix4 world = scaleMat * transMat * _parent->WorldTransform();

				// ビュー行列を取得し、最終行列を計算

				int tileRow = tileID / tilesPerRow;
				int tileCol = tileID % tilesPerRow;

				Vector2 texScale(_tileMapComp->TileSize() / static_cast<float>(_texture->Width()), _tileMapComp->TileSize() / static_cast<float>(_texture->Height()));

				Vector2 texOffset = Vector2(tileCol * texScale.x, tileRow * texScale.y); // 切り出す部分の左上

				// シェーダーに行列を設定
				shader->SetMatrixUniform("uWorldTransform", world);
				shader->SetVector2Uniform("uTexOffset", texOffset);
				shader->SetVector2Uniform("uTexScale", texScale);

				// テクスチャをアクティブにして描画
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

#pragma region プライベート関数



#pragma endregion

