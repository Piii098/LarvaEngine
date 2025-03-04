#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Components/Component.h"
#include "GameObjects/GameObject.h"

class BoxComponent2D;
template <typename T>
class AssetManager;
class TileMap;
class Texture;
class GameObject;
class SpriteComponent;
class DebugDrawComponent;

// タイルデータを保持する構造体
struct TileInfo {
	
    
    int id;                     // タイルID
	GameObject::TAG tag;        // タグ
    
    bool isCollider;            // 衝突判定の有無
    Vector3 lightColor;         // 発光色
    float lightIntensity;       // 発光強度

    TileInfo(int tileId = -1)
        : id(tileId)
		, tag(GameObject::TAG::NONE)
        , isCollider(false)
        , lightColor(1.0f, 1.0f, 1.0f)
        , lightIntensity(0.0f) {}
};

#pragma region タイル

class Tile : public GameObject {
public:
    Tile(Game* game, int tileId, const Vector2Int& position, const std::string& tileSetName);
    ~Tile() override;


    // タイルのプロパティ設定
    void SetCollider(bool hasCollider);
    void SetLight(const Vector3& color, float intensity);
    void SetTexOffset(const Vector2& offset);
    void SetTexScale(const Vector2& scale);
    // アクセサ
    int TileId() const { return _tileId; }
    void TileId(int tileId) { _tileId = tileId; }
	void TileSize(int size) { _tileSize = size; }
    BoxComponent2D* GetBoxComponent() const { return _boxComp; }

private:
    int _tileId;                // タイルID
    int _tileSize;              // タイルサイズ
    Vector2 _texOffset;
    BoxComponent2D* _boxComp;   // 衝突判定コンポーネント
    SpriteComponent* _sprite;   // 描画コンポーネント

};

#pragma endregion

class TileMapComponent : public Component {
public:

    TileMapComponent(GameObject* parent, int drawLayer = 75);
    ~TileMapComponent();

    void SetTileMap(const std::string& tileMapName, const std::string& tileSetName, int tileSetSize, int tileSize = 32); //ファイルでデータ読み込み
    void RegisterTileInfo(int tileId, bool isCollider, const Vector3& lightColor, float lightIntensity, GameObject::TAG objectTag);
    void CreateTiles();
	void DestroyTile(int tileId);

    /*ゲッターセッター*/

private:
    AssetManager<TileMap>* _tileMapManager;
    AssetManager<Texture>* _textureManager;
    Texture* _tileSet;
    std::string _tileSetName;
	int _tileSetSize;
    TileMap* _tileMap;
    int _mapWidth;
    int _mapHeight;
    int _tileSize;

    std::vector<TileInfo> _tileInfos;
};
