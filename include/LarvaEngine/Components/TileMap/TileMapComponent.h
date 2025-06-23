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

class Tile : public GameObject {
public:
    Tile(GameObject* parent, int x, int y, int TileID)
        : GameObject(parent), _x(x), _y(y), _tileID(TileID) {
    }
    ~Tile() override {}

    int TileID() const { return _tileID; }
    int X() const { return _x; }
    int Y() const { return _y; }

private:
    int _tileID;
    int _x;
    int _y;

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

    void SetCollision(int x, int y, bool isCollision);
    void SetCollision(int tileID, bool isCollision);

    const Vector2& GetTilePosition(int tileID) const;

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
    std::vector<Tile*> _tiles; // タイルオブジェクトのリスト

};