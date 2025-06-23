#pragma once
#include <vector>
#include <string>
#include <memory>

class VertexArray3D;
class Texture;
class Game;

class Mesh {
public:
	Mesh(Game& game);
	~Mesh();

	bool Load(const std::string& fileName);
	void Unload() {};
	
	VertexArray3D& GetVertexArray() const { return *_vertexArray; }

	void SetShaderName(const std::string& shaderName) { _shaderName = shaderName; }

	const std::string& GetShaderName() const { return _shaderName; }
	 
	Texture* GetTexture(size_t index);

	float GetRadius() const { return _radius; }

	float SpecularPower() const { return _specularPower; }

private:

	Game& _game; ///< �Q�[���N���X�ւ̎Q��

	std::vector<Texture*> _textures; ///< �e�N�X�`���̔z��
	VertexArray3D* _vertexArray;

	std::string _shaderName; ///< �V�F�[�_�[�̖��O
	float _radius; ///< ���a
	float _specularPower; ///< �X�y�L�����̋���
};