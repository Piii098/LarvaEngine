#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>
#include <SDL3/SDL_log.h>
#include <algorithm> 
#include "LarvaEngine/Core/Resources/Model.h"
#include "LarvaEngine/Renderer/3D/VertexArray3D.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/Resources/AssetManager.h"
#include "LarvaEngine/Core/Resources/Texture.h"
#include <gl/glew.h>

Model::Model(Game& game)
	: _game(game){
	_game.GetTextureManager().Load("BasicTexture", "Assets/Model/BasicTexture.png");
}

Model::~Model() {
}

bool Model::Load(const std::string& fileName) {
    // 初期化
    _meshes.clear();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_OptimizeMeshes |
        aiProcess_GenNormals |      // 法線生成
        aiProcess_FlipUVs |         // UVを反転（必要な場合）
        aiProcess_CalcTangentSpace  // タンジェント空間の計算（バンプマッピングなどに必要）
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->HasMeshes()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "ASSIMP Error: %s", importer.GetErrorString());
        return false;
    }

    // ルートノードから階層構造を処理
    aiMatrix4x4 rot;
    rot = aiMatrix4x4();
	rot = aiMatrix4x4::RotationX(AI_MATH_PI / 2.0f, rot);
    aiMatrix4x4 identity;
    ProcessNode(scene->mRootNode, scene, rot);

    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform) {
    // このノードの変換行列
    aiMatrix4x4 transform = parentTransform * node->mTransformation;

    // このノードのメッシュを処理
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(mesh, scene, transform));
    }

    // 子ノードを処理
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, transform);
    }
}

Model::Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& transform) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // 頂点データの抽出
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // 変換行列を頂点に適用
        aiVector3D pos = mesh->mVertices[i];
        pos = transform * pos;

        // 頂点位置
        vertices.push_back(pos.x);
        vertices.push_back(pos.y);
        vertices.push_back(pos.z);

        // 法線
        if (mesh->HasNormals()) {
            // 法線も変換する（スケーリングを考慮して正規化）
            aiVector3D normal = mesh->mNormals[i];
            aiMatrix3x3 normalMatrix(transform);
            normal = normalMatrix * normal;
            normal.Normalize();

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // テクスチャ座標
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    // インデックスデータの抽出
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // メッシュオブジェクトを作成して返す
    Mesh newMesh;
    newMesh.vertexArray = new VertexArray3D(vertices.data(),
        static_cast<unsigned int>(vertices.size() / 8),
        indices.data(),
        static_cast<unsigned int>(indices.size()));
    newMesh.numVerts = static_cast<unsigned int>(vertices.size() / 8);
    newMesh.numIndices = static_cast<unsigned int>(indices.size());

    // マテリアル情報の抽出（必要に応じて）
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
		aiString texturePath;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
            std::string baseDir = "Assets/Model/";
            std::string texName = texturePath.C_Str();
       
			std::string textureFile = baseDir + texturePath.C_Str();
            std::replace(textureFile.begin(), textureFile.end(), '\\', '/');
	        
			_game.GetTextureManager().Load(texName, textureFile);

			SDL_Log("Texture loaded: %s", textureFile.c_str());
			newMesh._textureName = texName;
        }
        else {
            newMesh._textureName = "BasicTexture";
        }
	
    }

    return newMesh;
}

void Model::Render() {
    for (const auto& mesh : _meshes) {
        mesh.vertexArray->SetActive();
        if (!mesh._textureName.empty()) {
			auto texture = _game.GetTextureManager().Get(mesh._textureName);
			if (texture) {
				texture->SetActive();
			}
        }
        glDrawElements(GL_TRIANGLES, mesh.numIndices, GL_UNSIGNED_INT, nullptr);
    }
}

void Model::Unload() {
    for (auto& mesh : _meshes) {
        delete mesh.vertexArray;
        mesh.vertexArray = nullptr;
    }
    _meshes.clear();
}
void Model::SetActive() {

}