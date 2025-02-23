#pragma once
#include <vector>
#include  "Components/SpriteComponent.h"


/*
*/

class Shader;
class TileMapComponent;

class TileMapSpriteComponent : public SpriteComponent {
public:	

	TileMapSpriteComponent(GameObject* parent, int drawLayer = 100);
	~TileMapSpriteComponent();

	void SetTileMapComponent(TileMapComponent* tileMapComp);

	void Draw(Shader* shader) override;

private:
	TileMapComponent* _tileMapComp;
};