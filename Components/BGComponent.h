#pragma once
#include "Components/TextureComponent.h"

class Texture;

class BGComponent : public TextureComponent {
public:
	BGComponent(GameObject* parent, float offsetRatio, int updateLayer = 100);
	~BGComponent();
	
	void Draw(Shader* shader) override;

private:
	float _offsetRatio;
};