#include "Singletons/ShaderManager.h"
#include "Utilities/Shader.h"

#pragma region �V���O���g��()

ShaderManager* ShaderManager::_instance = nullptr;

ShaderManager* ShaderManager::Instance() {
	return _instance;
}

void ShaderManager::Create() {
	if (_instance == nullptr) {
		_instance = new ShaderManager();
	}
}

void ShaderManager::Destroy() {
	delete _instance;
	_instance = nullptr;
}

#pragma endregion

#pragma region �R���X�g���N�^:�f�X�g���N�^

ShaderManager::ShaderManager() {

}


ShaderManager::~ShaderManager() {

}

#pragma endregion

#pragma region �p�u���b�N�֐�

bool ShaderManager::Load(const std::string& shaderName, const std::string& vertName, const std::string& fragName) {
	Shader* sha = nullptr;
	auto iter = _shaders.find(shaderName);
	if (iter == _shaders.end())  //�L�[�����݂��Ȃ��Ȃ�
	{

		sha = new Shader();

		if (sha->Load(vertName, fragName))
		{
			_shaders.emplace(shaderName, sha);
			return true;
		}
		else
		{
			delete sha;
			sha = nullptr;
			return false;
		}

	}

	return false;

}

void ShaderManager::Unload() {
	for (auto& pair : _shaders) {
		delete pair.second;
	}
	_shaders.clear();
}

Shader* ShaderManager::GetShader(const std::string& shaderName) {
	return _shaders[shaderName];
}


#pragma endregion

