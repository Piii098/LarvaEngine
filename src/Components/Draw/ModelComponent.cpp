#include "LarvaEngine/Components/Draw/ModelComponent.h"

#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Renderer/3D/VertexArray3D.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Model.h"
#include "LarvaEngine/Core/Resources/Texture.h"

ModelComponent::ModelComponent(GameObject& gameObject)
	: Component(gameObject)
	, _model(nullptr)
{
	_parent.GetMainScene().AddModel(this);
}

ModelComponent::~ModelComponent() {

	// Destructor implementation (if needed)
}

void ModelComponent::Render(Shader& shader) {
	if (_model) {

		Matrix4 worldTransform = GetParent().WorldTransform();
		shader.SetMatrixUniform("uWorldTransform", worldTransform);
		worldTransform.Invert();
		worldTransform.Transpose();
		shader.SetMatrixUniform("uNormalMatrix", worldTransform);

		//shader.SetFloatUniform("uSpecularPower", 1.0);
	
		_model->Render();
	}
}

void ModelComponent::SetModel(const std::string& modelName) {
	_model = GetParent().GetScene().GetManager().GetGame().GetModelManager().Get(modelName);
}

