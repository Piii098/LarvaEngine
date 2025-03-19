#include "LarvaEngine/Core/Resources/TileMap.h"
#include <fstream>
#include <sstream>

#pragma region コンストラクタ:デストラクタ

TileMap::TileMap()
    : _width(0)
    , _height(0) {
    // デフォルトのタイル情報を初期化
}

TileMap::~TileMap() {
    Unload();
}

#pragma endregion

#pragma region パブリック関数

bool TileMap::Load(const std::string& fileName) {
    _tileMaps.clear();
    std::ifstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> rowTiles;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            rowTiles.push_back(std::stoi(cell));
        }

        _tileMaps.push_back(rowTiles);
    }

    file.close();

    _height = static_cast<int>(_tileMaps.size());
    _width = _height > 0 ? static_cast<int>(_tileMaps[0].size()) : 0;

    return true; 

}

void TileMap::Unload() {
    _tileMaps.clear();
    _width = 0;
    _height = 0;
}

int TileMap::GetTileID(int x, int y) const {
    if (y >= 0 && y < static_cast<int>(_tileMaps.size())) {
        if (x >= 0 && x < static_cast<int>(_tileMaps[y].size())) {
            return _tileMaps[y][x];
        }
    }
    return -1; // 範囲外または無効な値
}

void TileMap::CreateEmptyMap(int width, int height, int defaultTileID) {
    _width = width;
    _height = height;

    _tileMaps.clear();
    _tileMaps.resize(height, std::vector<int>(width, defaultTileID));
}

void TileMap::SetTileID(int x, int y, int tileID) {
    if (x >= 0 && x < _width && y >= 0 && y < _height) {
        _tileMaps[y][x] = tileID;
    }
}

#pragma endregion