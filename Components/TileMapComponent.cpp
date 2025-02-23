#include <fstream>
#include <string>
#include <sstream>
#include "Components/TileMapComponent.h"
#include "Utilities/Texture.h"
#include "Utilities/Shader.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "GameObjects/Camera.h"
#include "Components/BoxComponent2D.h"

#pragma region タイルマップ管理クラス

TileMapComponent::TileMapManager::TileMapManager() {

}

void TileMapComponent::TileMapManager::Load(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		return;
	}

	std::vector<std::vector<int>> tempMap;


	std::string line;
	while (std::getline(file, line)) {
		std::vector<int> rowTiles;
		std::stringstream ss(line);
		std::string cell;

		while (std::getline(ss, cell, ',')) {
			rowTiles.push_back(std::stoi(cell));
		}

		tempMap.push_back(rowTiles);
	}

	_tileMaps[fileName] = std::move(tempMap);

	file.close();

	return;
}

void TileMapComponent::TileMapManager::Unload() {
	_tileMaps.clear();
}

const std::vector<std::vector<int>>& TileMapComponent::TileMapManager::GetTileMap(const std::string& tileMapName) {
	static const std::vector<std::vector<int>> emptyMap;  // 存在しない場合のデフォルト値

	auto it = _tileMaps.find(tileMapName);
	if (it != _tileMaps.end()) {
		return it->second;  // 参照を返す（コピーを防ぐ）
	}

	return emptyMap;  // 空のマップを返す（例外を防ぐ）
}

#pragma endregion

#pragma region コンストラクタ：デストラクタ

TileMapComponent::TileMapManager TileMapComponent::s_TileMapManager;

TileMapComponent::TileMapComponent(GameObject* parent, int drawLayer)
	: Component(parent)
	, _tileSize(32)
	, _mapWidth(0)
	, _mapHeight(0){

}

TileMapComponent::~TileMapComponent() {

}

#pragma endregion

#pragma region パブリック関数

void TileMapComponent::SetTileMap(const std::string& fileName, int tileSize) {
	
	_tiles = s_TileMapManager.GetTileMap(fileName);

	if (_tiles.empty()) {
		SDL_Log("Error: Tile map '%s' not found or empty.", fileName.c_str());
		_mapWidth = 0;
		_mapHeight = 0;
		return;
	}

	_mapWidth = _tiles[0].size();
	_mapHeight = _tiles.size();
	_tileSize = tileSize;
}

void TileMapComponent::SetTileMapData(std::vector<std::vector<int>> tiles, int width, int height, int tileSize) {
	_tileSize = tileSize;
	_mapWidth = width; 
	_mapHeight = height;

	_tiles.resize(_mapHeight, std::vector<int>(_mapWidth, 0));
}

#pragma endregion

#pragma region プライベート関数



#pragma endregion

