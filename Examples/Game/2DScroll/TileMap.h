#pragma once
#include "LarvaEngine/Core/GameObject.h"

namespace Example2DScroll
{

	class TileMap : public GameObject
	{
	public:
		TileMap(Scene& scene);
		~TileMap() override;

	};

}