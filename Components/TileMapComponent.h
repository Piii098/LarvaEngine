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

	#pragma region �^�C���}�b�v�Ǘ��N���X

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

	void SetTileMap(const std::string& fileName, int tileSize = 32); //�t�@�C���Ńf�[�^�ǂݍ���
	void SetTileMapData(std::vector<std::vector<int>> tiles, int width, int height, int tileSize = 32); // �z��Ńf�[�^�ǂݍ���

	/*�Q�b�^�[�Z�b�^�[*/
	
	static TileMapManager& S_TileMapManager() { return s_TileMapManager; }; // �ÓI�^�C���}�b�v�Ǘ�(��{���[�h�����̂�)

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