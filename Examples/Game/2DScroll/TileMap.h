#pragma once
#include "LarvaEngine/Core/GameObject.h"

class TileMapComponent;

namespace Example2DScroll
{

	class TileMap : public GameObject
	{
	public:
		TileMap(Scene& scene);
		~TileMap() override;

		void SetOnTile(int tileID, GameObject* obj);
	private:
		TileMapComponent* _tileMapComp;
	};

}