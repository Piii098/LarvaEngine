#pragma once
#include "GameObjects/GameObject.h"

class SpriteComponent;

class Background : public GameObject {
public:
	Background(Scene* scene);
	~Background() override;

public:
	std::vector<SpriteComponent*> _bgComps;

};