#pragma once
#include "GameObjects/GameObject.h"

class BGComponent;

class Background : public GameObject {
public:
	Background(Game* game);
	~Background();

public:
	BGComponent* _bgComp;

};