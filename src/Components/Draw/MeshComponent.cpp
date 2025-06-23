#include "LarvaEngine/Components/Draw/MeshComponent.h"
#include "LarvaEngine/Renderer/Shader.h"
#include "LarvaEngine/Renderer/3D/Mesh.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include "LarvaEngine/Renderer/3D/VertexArray3D.h"
#include "LarvaEngine/Core/SceneManager.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Renderer/3D/Mesh.h"

//==============================================================================
// �R���X�g���N�^�E�f�X�g���N�^
//==============================================================================

MeshComponent::MeshComponent(GameObject& parent, int updateLayer)
	: Component(parent, updateLayer)
	, _mesh(nullptr)
	, _textureIndex(0) {
	_parent.GetScene().AddMesh(this);
}

MeshComponent::~MeshComponent() {
	_parent.GetScene().RemoveMesh(this);
}

//==============================================================================
// �p�u���b�N�֐�
//==============================================================================

void MeshComponent::Render(Shader& shader) {
	if (_mesh) {

		shader.SetMatrixUniform("uWorldTransform", GetParent().WorldTransform());
		shader.SetFloatUniform("uSpecularPower", _mesh->SpecularPower());

		Texture* tex = _mesh->GetTexture(_textureIndex);
		if (tex) {
			tex->SetActive();
		}

		VertexArray3D& va = _mesh->GetVertexArray();
		va.SetActive();

		glDrawElements(GL_TRIANGLES, va.NumIndices(), GL_UNSIGNED_INT, nullptr);

	}
}

void MeshComponent::SetMesh(const std::string& meshName) {
	_mesh = GetParent().GetScene().GetManager().GetGame().GetMeshManager().Get(meshName);

}