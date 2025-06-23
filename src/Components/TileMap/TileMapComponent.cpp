#include "LarvaEngine/Components/TileMap/TileMapComponent.h"
#include "LarvaEngine/Components/Physics/Box2DComponent.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Components/Draw/SpriteComponent.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Resources/TileMap.h"

TileMapComponent::TileMapComponent(GameObject& parent, int tileSize, const std::string& textureName)
    : Component(parent)
    , _tileSize(tileSize)
    ,_textureName(textureName)
{

}

TileMapComponent::~TileMapComponent() {
    //DestroyTileObjects();
}

void TileMapComponent::SetTileMap(const std::string& tileMapName) {
    //DestroyTileObjects();

    _tileMap = _parent.GetScene().GetManager().GetGame().GetTileMapManager().Get(tileMapName);

    CreateTileObjects();
}

void TileMapComponent::SetCollision(int x, int y, bool isCollision) {
	for (auto* obj : _tiles) {
        if (obj->X() == x && obj->Y() == y) {
			auto& boxComp = obj->CreateComponent<Box2DComponent>(isCollision, false);
			AABB2D box(Vector2(-_tileSize / 2, -_tileSize / 2), Vector2(_tileSize / 2, _tileSize / 2));
			boxComp.SetObjectBox(box);
            boxComp.DisplayBox();
        }
	}
}

void TileMapComponent::SetCollision(int tileID, bool isCollision) {
	for (auto* obj : _tiles) {
		if (obj->TileID() == tileID) {
            auto& boxComp = obj->CreateComponent<Box2DComponent>(isCollision, false);
            AABB2D box(Vector2(-_tileSize / 2, -_tileSize / 2), Vector2(_tileSize/2, _tileSize / 2));
            boxComp.SetObjectBox(box);
			// boxComp.DisplayBox();
            obj->Tag(GameObject::TAG::GROUND);
		}
	}
}


void TileMapComponent::CreateTileObjects() {
    int width = _tileMap->Width();
    int height = _tileMap->Height();

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int tileID = _tileMap->GetTileID(x, y); // タイルIDを取得
            if (tileID < 0) continue; // 負値なら空タイル等としてスキップ

            //子オブジェクトを生成
            auto& tileObj = GetParent().GetScene().CreateChildObject<Tile>(&_parent, x, y, tileID);

            // タイルIDからテクスチャのオフセットを計算（例：横10枚タイルシート想定）
            int sheetCols = 4; // タイルシートの1行のタイル数。適宜変更
            int sheetRows = 4; // タイルシートの1列のタイル数。適宜変更
            float tx = tileID % sheetCols; // タイルシート内のX
            float ty = tileID / sheetCols; // タイルシート内のY

			if (tx >= sheetCols || ty >= sheetRows) {
                Vector2Int pos = Vector2Int(x * _tileSize + _tileSize / 2, (height - 1 - y) * _tileSize + _tileSize / 2);
                SDL_Log("Tile Position: %d, %d", pos.x, pos.y); // 確認用ログ
                tileObj.Position(Vector3(pos.x, pos.y, 0));
                _tiles.push_back(&tileObj);
				continue; // タイルIDが範囲外の場合はスキップ
			}
			
            // スプライトコンポーネントを付与
            auto& sprite = tileObj.CreateComponent<SpriteComponent>(10);
            sprite.SetTexture(_textureName);

            Vector2 offset = Vector2(
                static_cast<float>(tx / sheetCols),
                static_cast<float>(ty / sheetRows)
            );
            Vector2 scale = Vector2(
                static_cast<float>(1.f / sheetCols),
                static_cast<float>(1.f / sheetRows)
            );

            sprite.TexOffset(offset);
            sprite.TexScale(scale);
			//sprite.Alpha(0.5f);

            Vector2Int pos = Vector2Int(x * _tileSize + _tileSize / 2, (height - 1 - y) * _tileSize + _tileSize / 2);
            tileObj.Position(Vector3(pos.x, pos.y, 0));
			_tiles.push_back(&tileObj);
        }
    }
}

void TileMapComponent::DestroyTileObjects() {
   // for (auto* obj : _tileObjects) {
        //GetParent().GetScene().DestroyObject(obj);
    //}
    //_tileObjects.clear();
}

const Vector2Int& TileMapComponent::GetTilePosition(int tileID) const {
	for (auto* obj : _tiles) {
		if (obj->TileID() == tileID) {
            _parent.ComputeWorldTransform();
            // 方法1: WorldTransformを直接使用する（推奨）
            const Matrix4& worldTransform = obj->WorldTransform();
            // 原点（0,0,0）を変換することでオブジェクトの位置を取得
            Vector3 originPoint = Vector3::Zero;
            Vector3 worldPos = Vector3::Transform(originPoint, worldTransform);
            return Vector2Int(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y));
		}
	}

	return Vector2Int(-1, -1); // タイルが見つからなかった場合
}