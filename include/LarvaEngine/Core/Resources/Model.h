#pragma once
#include <string>
#include <vector>
#include <memory>
#include <assimp/matrix4x4.h>
#include "LarvaEngine/Core/Utilities/Math.h"

namespace Assimp {
	class Importer;
};

class Game;
class VertexArray3D;
class Texture;
struct aiMesh;
struct aiNode;
struct aiScene;


class Model {
public:
	Model(Game& game);
	~Model();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	void Render();


private:
	Game& _game;

	struct Mesh {
		VertexArray3D* vertexArray;
		unsigned int numVerts;
		unsigned int numIndices;
		std::string _textureName;
	};

	std::vector<Mesh> _meshes;
	
	void ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform);

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& transform);

};