#pragma once
#include <string>
#include <vector>

class  TileMap {
public:
	TileMap();
	~TileMap();

	bool Load(const std::string& fileName);
	void Unload();

    int Width() const { return _width; }
    int Height() const { return _height; }

	int GetTileID(int x, int y) const;
	const std::vector<std::vector<int>>& GetTileMaps() const { return _tileMaps; }

    void CreateEmptyMap(int width, int height, int defaultTileID);
    void SetTileID(int x, int y, int tileID);

private:
	int _width;
	int _height;

	std::vector<std::vector<int>> _tileMaps;

};