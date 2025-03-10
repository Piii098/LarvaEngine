#include "Components/TileMapComponent.h"
#include "Components/Phys/BoxComponent2D.h"
#include "Components/Draw/DebugDrawComponent.h"
#include "Components/Draw/SpriteComponent.h"
#include "AssetManagers/AssetData/Texture.h"
#include "Renderer/Shader.h"
#include "Core/Game.h"
#include <fstream>
#include <sstream>
#include "AssetManagers/AssetManager.h"
#include "AssetManagers/AssetData/TileMap.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#pragma region タイルクラス

Tile::Tile(Scene* scene, int tileId, const Vector2Int& position, const std::string& tileSetName)
    : GameObject(scene)
    , _tileId(tileId)
    , _tileSize(0)
    , _texOffset(Vector2::Zero)
    , _boxComp(nullptr)
    , _sprite(nullptr) {
    Position(position);
	Scale(1.f);
    _sprite = new SpriteComponent(this, 10,10);
    _sprite->SetTexture(tileSetName);

    // デフォルトでは衝突なし
    _boxComp = nullptr;
}

Tile::~Tile() {
    if (_boxComp) {
        delete _boxComp;
        _boxComp = nullptr;
    }
    delete _sprite;
    _sprite = nullptr;
    // コンポーネントはGameObjectが自動的に削除する
}

void Tile::SetCollider(bool hasCollider) {
    if (hasCollider && !_boxComp) {
        // 衝突判定コンポーネントの生成
        _boxComp = new BoxComponent2D(this, true, false);
                
        // タイルの中心を原点として当たり判定範囲を設定
        int halfTile = _tileSize / 2;
        Vector2Int boxMin(static_cast<int>(-halfTile), static_cast<int>(-halfTile));
        Vector2Int boxMax(static_cast<int>(halfTile), static_cast<int>(halfTile));
        
                 
        _boxComp->SetObjectBox(AABB2D(boxMin, boxMax));

        // デバッグ用の描画コンポーネント追加
        new DebugDrawComponent(this, _boxComp);
    }
    else if (!hasCollider && _boxComp) {
        RemoveComponent(_boxComp);
        _boxComp = nullptr;
    }
}

void Tile::SetTexOffset(const Vector2& offset) {
    _sprite->TexOffset(offset);
};

void Tile::SetTexScale(const Vector2& scale) {
	_sprite->TexScale(scale);
};

void Tile::SetLight(const Vector3& color, float intensity) {
    _sprite->SelfLightColor(color);
    _sprite->SelfLightIntensity(intensity);
}

#pragma endregion

#pragma region コンストラクタ:デストラクタ

TileMapComponent::TileMapComponent(GameObject* parent, int drawLayer)
    : Component(parent, drawLayer)
    , _tileMapManager(nullptr)
    , _textureManager(nullptr)
    , _tileSetName("")
    , _tileMap(nullptr)
	, _tileSet(nullptr)
    , _mapWidth(0)
    , _mapHeight(0)
    , _tileSize(32) {
    _tileInfos.resize(256);
    _tileMapManager = parent->GetScene()->GetManager()->GetGame()->GetTileMapManager();
    _textureManager = parent->GetScene()->GetManager()->GetGame()->GetTextureManager();
}

TileMapComponent::~TileMapComponent() {
}

#pragma endregion

#pragma region パブリック関数 

void TileMapComponent::SetTileMap(const std::string& tileMapName, const std::string& tileSetName, int tileSetSize, int tileSize) {
    _tileMap = _tileMapManager->Get(tileMapName);
    _tileSize = tileSize;
	_tileSet = _textureManager->Get(tileSetName);
    _tileSetName = tileSetName;
    _tileSetSize = tileSetSize;
}

void TileMapComponent::RegisterTileInfo(int tileId, bool isCollider, const Vector3& lightColor, float lightIntensity, GameObject::TAG objectTag) {
    if (tileId < 0) return;

    // タイル情報を設定
    _tileInfos[tileId].id = tileId;
    _tileInfos[tileId].tag = objectTag;
    _tileInfos[tileId].isCollider = isCollider;
    _tileInfos[tileId].lightColor = lightColor;
    _tileInfos[tileId].lightIntensity = lightIntensity;
}

void TileMapComponent::CreateTiles() {
    if (!_tileMap) return;

    // マップの幅と高さを取得
    _mapWidth = _tileMap->Width();
    _mapHeight = _tileMap->Height();

    int tilePerRow = _tileSet->Width() / _tileSize;
    int tilePerColumn = _tileSet->Height() / _tileSize;

    // タイルを生成
    for (int y = _mapHeight - 1; y >= 0; --y) {
        for (int x = 0; x < _mapWidth; ++x) {
            int tileId = _tileMap->GetTileID(x, y);
			if (tileId > _tileSetSize) continue;
            if (tileId < -1) {
                Vector2Int position(x * _tileSize + GetParent()->Position().x, (_mapHeight - 1 - y) * _tileSize + GetParent()->Position().y);
                auto tile = GetParent()->GetScene()->CreateChildObject<Tile>(GetParent(), tileId, Vector2Int(x * _tileSize, (_mapHeight - 1 - y) * _tileSize), _tileSetName);
                Vector2 offset = Vector2(0, 0);
                tile->SetTexOffset(offset);
                Vector2 scale = Vector2(1.0f / tilePerRow, 1.0f / tilePerColumn);
                tile->SetTexScale(scale);
                tile->Scale(1.0f); // スケールを1.0に設定
            }else if (tileId != -1 && tileId < _tileInfos.size()) { // 有効なタイルIDの場合
                Vector2Int position(x * _tileSize + GetParent()->Position().x, (_mapHeight - 1 - y) * _tileSize + GetParent()->Position().y);
                auto tile = GetParent()->GetScene()->CreateChildObject<Tile>(GetParent(),tileId, position, _tileSetName);
                tile->TileSize(_tileSize);
                tile->Tag(_tileInfos[tileId].tag);
                tile->SetCollider(_tileInfos[tileId].isCollider);
                tile->SetLight(_tileInfos[tileId].lightColor, _tileInfos[tileId].lightIntensity);

                int row = tileId / tilePerRow;
                int col = tileId % tilePerRow;

                Vector2 offset = Vector2(static_cast<float>(col) / tilePerRow, static_cast<float>(row) / tilePerColumn);
                tile->SetTexOffset(offset);
                Vector2 scale = Vector2(1.0f / tilePerRow, 1.0f / tilePerColumn);
                tile->SetTexScale(scale);
                tile->Scale(1.0f); // スケールを1.0に設定

                // offsetとscaleの値をログに出力
            }
        }
    }
}

void TileMapComponent::DestroyTile(int tileId) {
    if (!_tileMap) return;

	for (auto& tile : GetParent()->GetChildren()) {
		Tile* tileObject = static_cast<Tile*>(tile);
        if (tileObject && tileObject->TileId() == tileId) {
			GetParent()->GetScene()->RemoveObject(tileObject);
			break;
		}
	}
}
