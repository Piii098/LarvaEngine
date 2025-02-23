#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "Components/Component.h"
/*

*/

class BoxComponent2D;

class TileMapComponent : public Component {
public:

	#pragma region タイルマップ管理クラス

	class TileMapManager {
	public:
		TileMapManager();
		~TileMapManager() = default;

		void Load(const std::string& fileName);
		void Unload();

		const std::vector<std::vector<int>>& GetTileMap(const std::string& tileMapName);
	private:

		std::unordered_map<std::string, std::vector<std::vector<int>>> _tileMaps;

	};

	#pragma endregion

	TileMapComponent(GameObject* parent, int drawLayer = 75);
	~TileMapComponent();

	void SetTileMap(const std::string& fileName, int tileSize = 32); //ファイルでデータ読み込み
	void SetTileMapData(std::vector<std::vector<int>> tiles, int width, int height, int tileSize = 32); // 配列でデータ読み込み

	/*ゲッターセッター*/
	
	static TileMapManager& S_TileMapManager() { return s_TileMapManager; }; // 静的タイルマップ管理(基本ロード処理のみ)

	std::vector<std::vector<int>> Tiles() { return _tiles; };
	int TileSize() { return _tileSize; };
	int MapWidth() { return _mapWidth; };
	int MapHeight() { return _mapHeight; };

private:

	static TileMapManager s_TileMapManager;

	std::vector<std::vector<int>> _tiles;
	int _tileSize;
	int _mapWidth;
	int _mapHeight;
};