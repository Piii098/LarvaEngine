#include <fstream>
#include <string>
#include <sstream>
#include <SDL3/SDL.h>
#include "Utilities/Shader.h"

#pragma region �R���X�g���N�^:�f�X�g���N�^

Shader::Shader()
	: _shaderProgram(0)
	, _vertexShader(0)
	, _fragShader(0){

}

Shader::~Shader() {

}

#pragma endregion

#pragma region �p�u���b�N�֐�

bool Shader::Load(const std::string& vertName
	, const std::string& fragName) {

	/*�V�F�[�_�[�R���p�C��*/

	if (!CompileShader(vertName, GL_VERTEX_SHADER, _vertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, _fragShader))
	{
		return false;
	}

	/*�V�F�[�_�[�v���O�����쐬*/

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragShader);
	glLinkProgram(_shaderProgram);

	if (!IsValidProgram()) {
		return false;
	}

	return true;

}

void Shader::Unload() {
	glDeleteProgram(_shaderProgram);
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragShader);
}

void Shader::SetActive() {
	glUseProgram(_shaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix) {

	GLuint loc = glGetUniformLocation(_shaderProgram, name); // ���O��T��

	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr()); //
}

void Shader::SetVector3Uniform(const char* name, const Vector3& vec) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vec) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform2f(loc, vec.x, vec.y);
}

void Shader::SetFloatUniform(const char* name, float value) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform1f(loc, value);
}
void Shader::SetIntUniform(const char* name, int value) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform1i(loc, value);
}
void Shader::SetBoolUniform(const char* name, bool flag) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform1i(loc, static_cast<int>(flag));
}


#pragma endregion


#pragma region �v���C�x�[�g�֐�

bool Shader::CompileShader(const std::string& fileName
	, GLuint shaderType, GLuint& outShader) {

	std::ifstream shaderFile(fileName); // �t�@�C�����J��
	
	if (shaderFile.is_open()) { //�t�@�C�����J������

		/*�t�@�C���e�L�X�g�ǂݍ���*/
		
		std::stringstream sstream;
		sstream << shaderFile.rdbuf(); 
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		/*�V�F�[�_�[�쐬�A�R���p�C��*/

		outShader = glCreateShader(shaderType); // �V�F�[�_�[�^�C�v�w��A����
		glShaderSource(outShader, 1, &(contentsChar), nullptr); // �e�L�X�g���R���p�C��
		glCompileShader(outShader);

		if (!IsCompiled(outShader)) { // 
			SDL_Log("Failed to compile shader '%s'", fileName.c_str());
			return false;
		}
	}
	else {
		SDL_Log("Didn't find shader file '%s'", fileName.c_str());
		return false;
	}
	return true;
}

bool Shader::IsCompiled(GLuint shader) {

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // �V�F�[�_�[��Ԗ₢���킹

	if (status != GL_TRUE) { // �V�F�[�_�[��Ԃ�TRUE�łȂ��Ƃ�

		/*�R���p�C���G���[���b�Z�[�W�o��*/

		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer); 
		SDL_Log("Failed to compile GLSL :\n%s", buffer);
		return false;
	}
	return true;
}

bool Shader::IsValidProgram() {

	GLint status;
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &status); // �V�F�[�_�[�v���O������Ԗ₢���킹

	if (status != GL_TRUE) { // �V�F�[�_�[�v���O������Ԃ�TRUE�łȂ��Ƃ�

		/*�V�F�[�_�[�v���O�����G���[���b�Z�[�W�o��*/

		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(_shaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}

#pragma endregion
