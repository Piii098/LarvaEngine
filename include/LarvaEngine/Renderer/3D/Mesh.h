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

	Game& _game; ///< ゲームクラスへの参照

	std::vector<Texture*> _textures; ///< テクスチャの配列
	VertexArray3D* _vertexArray;

	std::string _shaderName; ///< シェーダーの名前
	float _radius; ///< 半径
	float _specularPower; ///< スペキュラの強さ
};