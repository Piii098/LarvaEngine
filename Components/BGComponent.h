#pragma once
#include "Components/TextureComponent.h"

class Texture;

class BGComponent : public TextureComponent {
public:
	BGComponent(GameObject* parent);
	~BGComponent();
	
	void Draw(Shader* shader) override;

private:

};