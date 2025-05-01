#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "LarvaEngine/Core/Component.h"
#include "LarvaEngine/Core/GameObject.h"

class TileMap;
class SpriteComponent;
class Texture;
class Scene;
class GameObject;

class Tile : public GameObject {
public:
	Tile(GameObject& parent, int TileID)
		: GameObject(parent), _tileID(TileID) {}
	~Tile() override = default;

private:
    int _tileID;

};

/// @brief タイルマップを描画・管理するコンポーネント
class TileMapComponent : public Component {
public:
    /// @brief タイルマップコンポーネントのコンストラクタ
    /// @param parent 親となるゲームオブジェクト
    /// @param scene 所属するシーン
    /// @param tileMap タイルマップデータ
    /// @param tileWidth タイル1枚の幅（ピクセル）
    /// @param tileHeight タイル1枚の高さ（ピクセル）
    /// @param textureName タイルセット画像のテクスチャ名
    TileMapComponent(GameObject& parent, int tileSize, const std::string& textureName);

    virtual ~TileMapComponent();

    /// @brief タイルマップデータを再設定
    void SetTileMap(const std::string& tileMapName);

    /// @brief タイルセットのテクスチャ名を取得
    std::string TextureName() const { return _textureName; }

private:
    /// @brief タイル毎に子オブジェクト・スプライトを生成
    void CreateTileObjects();

    /// @brief すべての子タイルオブジェクトを削除
    void DestroyTileObjects();

    // タイルマップデータ
    TileMap* _tileMap;
    int _tileSize;
    std::string _textureName;

    std::vector<Tile*> _tileObjects;
};