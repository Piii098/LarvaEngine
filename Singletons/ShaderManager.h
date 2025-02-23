#include <unordered_map>
#include <string>
#include "Components/Component.h"

class Shader;

class ShaderManager {
public:
	bool Load(const std::string& shaderName, const std::string& vertName, const std::string& fragName);
	void Unload();

	Shader* GetShader(const std::string& shaderName);

private:
	std::unordered_map<std::string, Shader*> _shaders;


	/*�V���O���g��*/

public:
	
	static void Create();
	static void Destroy();
	static ShaderManager* Instance();

private:

	ShaderManager(); //new�j�~
	ShaderManager(const ShaderManager&) = delete; //�R�s�[�R���X�g���N�^�j�~
	~ShaderManager(); //delete�j�~
	static ShaderManager* _instance;

};
