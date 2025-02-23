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

#pragma region �^�C���}�b�v�Ǘ��N���X

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
	static const std::vector<std::vector<int>> emptyMap;  // ���݂��Ȃ��ꍇ�̃f�t�H���g�l

	auto it = _tileMaps.find(tileMapName);
	if (it != _tileMaps.end()) {
		return it->second;  // �Q�Ƃ�Ԃ��i�R�s�[��h���j
	}

	return emptyMap;  // ��̃}�b�v��Ԃ��i��O��h���j
}

#pragma endregion

#pragma region �R���X�g���N�^�F�f�X�g���N�^

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

#pragma region �p�u���b�N�֐�

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

#pragma region �v���C�x�[�g�֐�



#pragma endregion

