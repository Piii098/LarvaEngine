#include <fstream>
#include <string>
#include <sstream>
#include <SDL3/SDL.h>
#include "Utilities/Shader.h"

#pragma region コンストラクタ:デストラクタ

Shader::Shader()
	: _shaderProgram(0)
	, _vertexShader(0)
	, _fragShader(0){

}

Shader::~Shader() {

}

#pragma endregion

#pragma region パブリック関数

bool Shader::Load(const std::string& vertName
	, const std::string& fragName) {

	/*シェーダーコンパイル*/

	if (!CompileShader(vertName, GL_VERTEX_SHADER, _vertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, _fragShader))
	{
		return false;
	}

	/*シェーダープログラム作成*/

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

	GLuint loc = glGetUniformLocation(_shaderProgram, name); // 名前を探す

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
void Shader::SetVector2Uniform(const char* name, const Vector2Int& vec) {
	Vector2 vecFlaot = Vector2::ToFloat(vec);
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform2f(loc, vecFlaot.x, vecFlaot.y);
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


#pragma region プライベート関数

bool Shader::CompileShader(const std::string& fileName
	, GLuint shaderType, GLuint& outShader) {

	std::ifstream shaderFile(fileName); // ファイルを開く
	
	if (shaderFile.is_open()) { //ファイルを開けたら

		/*ファイルテキスト読み込み*/
		
		std::stringstream sstream;
		sstream << shaderFile.rdbuf(); 
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		/*シェーダー作成、コンパイル*/

		outShader = glCreateShader(shaderType); // シェーダータイプ指定、制作
		glShaderSource(outShader, 1, &(contentsChar), nullptr); // テキストをコンパイル
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
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status); // シェーダー状態問い合わせ

	if (status != GL_TRUE) { // シェーダー状態がTRUEでないとき

		/*コンパイルエラーメッセージ出力*/

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
	glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &status); // シェーダープログラム状態問い合わせ

	if (status != GL_TRUE) { // シェーダープログラム状態がTRUEでないとき

		/*シェーダープログラムエラーメッセージ出力*/

		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(_shaderProgram, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}

	return true;
}

#pragma endregion
