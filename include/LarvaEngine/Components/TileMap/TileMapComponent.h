#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/GameObject.h"

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
    float lightIntensity;                                                                                // 発光強度

    TileInfo(int tileId = -1)
        : id(tileId)
		, tag(GameObject::TAG::NONE)
        , isCollider(false)
        , lightColor(1.0f, 1.0f, 1.0f)
        , lightIntensity(0.0f) {}
};

class TileMapComponent : public Component {
public:

    TileMapComponent(GameObject& parent, int drawLayer = 75);
    ~TileMapComponent();

    void SetTileMap(const std::string& tileMapName, const std::string& tileSetName, int tileSetSize, int tileSize = 32); //ファイルでデータ読み込み
    void RegisterTileInfo(int tileId, bool isCollider, const Vector3& lightColor, float lightIntensity, GameObject::TAG objectTag);
    void CreateTiles();
	void DestroyTile(int tileId);

    /*ゲッターセッター*/

private:

    AssetManager<TileMap>& _tileMapManager;
    AssetManager<Texture>& _textureManager;
    Texture* _tileSet;
    std::string _tileSetName;
	int _tileSetSize;
    TileMap* _tileMap;
    int _mapWidth;
    int _mapHeight;
    int _tileSize;

    std::vector<TileInfo> _tileInfos;
};
