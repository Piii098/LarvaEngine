#include "LarvaEngine/Components/TileMap/TileMapComponent.h"
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
    DestroyTileObjects();
}

void TileMapComponent::SetTileMap(const std::string& tileMapName) {
    DestroyTileObjects();

    _tileMap = _parent.GetScene().GetManager().GetGame().GetTileMapManager().Get(tileMapName);

    CreateTileObjects();
}
void TileMapComponent::CreateTileObjects() {
    int width = _tileMap->Width();
    int height = _tileMap->Height();

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            int tileID = _tileMap->GetTileID(x, y); // タイルIDを取得
            if (tileID < 0) continue; // 負値なら空タイル等としてスキップ
            SDL_Log("TileID: %d", tileID); // デバッグ用ログ出力

            // 子オブジェクトを生成
            auto& tileObj = GetParent().GetScene().CreateChildObject<Tile>(&_parent, tileID);

            // スプライトコンポーネントを付与
            auto& sprite = tileObj.CreateComponent<SpriteComponent>(10);
            sprite.SetTexture(_textureName);

            // タイルIDからテクスチャのオフセットを計算（例：横10枚タイルシート想定）
            int sheetCols = 4; // タイルシートの1行のタイル数。適宜変更
            int sheetRows = 4; // タイルシートの1列のタイル数。適宜変更
            int tx = tileID % sheetCols; // タイルシート内のX
            int ty = tileID / sheetCols; // タイルシート内のY
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

            tileObj.Position(Vector2Int(x * _tileSize + _tileSize / 2, (height - 1 - y) * _tileSize + _tileSize / 2));

            _tileObjects.push_back(&tileObj);
        }
    }
}

void TileMapComponent::DestroyTileObjects() {
    for (auto* obj : _tileObjects) {
        GetParent().GetScene().DestroyObject(obj);
    }
    _tileObjects.clear();
}
