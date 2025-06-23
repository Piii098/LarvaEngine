#pragma once
#include <string>
#include <vector>

/*
使用方法
TileMapクラスは、タイルマップの読み込み、アンロード、タイルの取得、設定を行うためのクラスです。
*/

class Game;

class TileMap {
public:
	TileMap(Game& game); // コンストラクタ
	~TileMap(); // デストラクタ

	// 指定されたファイル名からタイルマップを読み込む
	bool Load(const std::string& fileName);

	// タイルマップをアンロードする
	void Unload();

	// マップの幅を取得する
	int Width() const { return _width; }

	// マップの高さを取得する
	int Height() const { return _height; }

	// 指定された座標のタイルIDを取得する
	int GetTileID(int x, int y) const;

	// タイルマップの2次元ベクターを取得する
	const std::vector<std::vector<int>>& GetTileMaps() const { return _tileMaps; }

	// 指定された幅と高さで空のマップを作成する
	void CreateEmptyMap(int width, int height, int defaultTileID);

	// 指定された座標にタイルIDを設定する
	void SetTileID(int x, int y, int tileID);

private:

	Game& _game; // ゲームクラスへの参照

	int _width; // マップの幅
	int _height; // マップの高さ

	std::vector<std::vector<int>> _tileMaps; // タイルマップデータ
};