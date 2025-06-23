#pragma once
#include <string>
#include "LarvaEngine/Core/Component.h"

class Mesh;

class MeshComponent : public Component {
public: 
	MeshComponent(GameObject& parent, int updateLayer = 30);
	~MeshComponent() override;

	virtual void Render(Shader& shader);
	virtual void SetMesh(const std::string& meshName);
	void SetTextureIndex(int index) { _textureIndex = index; }

private:
	Mesh* _mesh;
	size_t _textureIndex;
};