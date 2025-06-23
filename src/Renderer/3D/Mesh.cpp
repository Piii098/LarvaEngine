#include <fstream>
#include <sstream>
#include <SDL3/SDL_log.h>
#include <rapidjson/document.h>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Renderer/3D/Mesh.h"
#include "LarvaEngine/Renderer/3D/VertexArray3D.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"


Mesh::Mesh(Game& game)
	: _game(game)
	, _vertexArray(nullptr)
	, _radius(0.f){

}

Mesh::~Mesh() {
}

bool Mesh::Load(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		SDL_Log("Failed to open file: %s", fileName.c_str());
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject()) {
		SDL_Log("Invalid JSON format in file: %s", fileName.c_str());
		return false;
	}

	int ver = doc["version"].GetInt();

	if (ver != 1) {
		SDL_Log("Invalid version in file: %s", fileName.c_str());
		return false;
	}

	_shaderName = doc["shader"].GetString();

	size_t vertSize = 8;

	const rapidjson::Value& textures = doc["textures"];
	if (!textures.IsArray() || textures.Size() < 1) {
		SDL_Log("Invalid textures in file: %s", fileName.c_str());
		return false;
	}

	_specularPower = doc["specularPower"].GetDouble();

	for (rapidjson::SizeType i = 0; i < textures.Size(); ++i) {
		const rapidjson::Value& texture = textures[i];
		if (!texture.IsString()) {
			SDL_Log("Invalid texture data in file: %s", fileName.c_str());
			return false;
		}

		std::string texName = texture.GetString();
		_game.GetTextureManager().Load(texName, texName);
		Texture* tex = _game.GetTextureManager().Get(texName);
		if (!tex) {
			// デフォルトテクスチャを試す
			tex = _game.GetTextureManager().Get("デフォルトテクスチャのパス");
			if (!tex) {
				SDL_Log("Warning: Failed to load texture: %s", texName.c_str());
				// 処理は続行する
			}
		}
		_textures.emplace_back(tex);
	}

	const rapidjson::Value& vertsJson = doc["vertices"];
	if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
		SDL_Log("Invalid vertices in file: %s", fileName.c_str());
		return false;
	}

	std::vector<float> vertices;
	vertices.reserve(vertsJson.Size() * vertSize);
	_radius = 0.f;
	for (rapidjson::SizeType i = 0; i < vertsJson.Size(); ++i) {
		const rapidjson::Value& vert = vertsJson[i];
		if (!vert.IsArray() || vert.Size() != vertSize) {
			SDL_Log("Invalid vertex data in file: %s", fileName.c_str());
			return false;
		}

		Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
		_radius = Math::Max(_radius, pos.LengthSq());

		for (rapidjson::SizeType j = 0; j < vert.Size(); ++j) {
			vertices.emplace_back(static_cast<float>(vert[j].GetDouble()));
		}
	}

	_radius = Math::Sqrt(_radius);

	const rapidjson::Value& indicesJson = doc["indices"];
	if (!indicesJson.IsArray() || indicesJson.Size() < 1) {
		SDL_Log("Invalid indices in file: %s", fileName.c_str());
		return false;
	}

	std::vector<unsigned int> indices;
	indices.reserve(indicesJson.Size() * 3);
	for (rapidjson::SizeType i = 0; i < indicesJson.Size(); ++i) {
		const rapidjson::Value& index = indicesJson[i];
		if (!index.IsArray() || index.Size() != 3) {
			SDL_Log("Invalid index data in file: %s", fileName.c_str());
			return false;
		}

		indices.emplace_back(index[0].GetUint());
		indices.emplace_back(index[1].GetUint());
		indices.emplace_back(index[2].GetUint());
	}

	_vertexArray = new VertexArray3D(vertices.data(), static_cast<unsigned int>(vertices.size() / vertSize),
		indices.data(), static_cast<unsigned int>(indices.size()));
	return true;
	
}

Texture* Mesh::GetTexture(size_t index) {
	if (index < _textures.size()) {
		return _textures[index];
	}
	else {
		SDL_Log("Invalid texture index: %zu", index);
		return nullptr;
	}
}