#pragma once
#include "LarvaEngine/Core/Component.h"
#include <string>

class Shader;
class Model;

class ModelComponent : public Component{
public:
	ModelComponent(GameObject& gameObject);
	~ModelComponent() override;

	void Render(Shader& shader);
	void SetModel(const std::string& modelName);

private:
	Model* _model;
};