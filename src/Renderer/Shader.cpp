#include <fstream>
#include <string>
#include <sstream>
#include <SDL3/SDL.h>
#include "LarvaEngine/Renderer/Shader.h"

//==============================================================================
// コンストラクタ・デストラクタ
//==============================================================================

Shader::Shader()
	: _shaderProgram(0)
	, _vertexShader(0)
	, _fragShader(0){

}

Shader::~Shader() {

}


//==============================================================================
// パブリック関数
//==============================================================================


// ===== ロード・アンロード =====//

/**
 * シェーダーファイルを読み込む
 * 
 * 二つのシェーダーファイルをメンバ関数を使って読み込み、コンパイルし
 * シェーダープログラムを作成する
 */
bool Shader::Load(const std::string& vertName, const std::string& fragName) {

	// シェーダーファイルを読み込み、コンパイル
	if (!CompileShader(vertName, GL_VERTEX_SHADER, _vertexShader) ||
		!CompileShader(fragName, GL_FRAGMENT_SHADER, _fragShader)){
		return false;
	}

	
	// シェーダープログラムを作成
	// 二つのシェーダーをアタッチし、リンクする
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragShader);
	glLinkProgram(_shaderProgram);

	if (!IsValidProgram()) { // シェーダープログラムが正常にリンクされているかチェック
		return false;
	}

	return true;

}

/**
 * シェーダーファイルをアンロードする
 *
 * シェーダープログラムを削除する
 */
void Shader::Unload() {
	glDeleteProgram(_shaderProgram);
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragShader);
}


// ===== アクティブ化 =====//

/**
 * シェーダープログラムをアクティブにする
 */
void Shader::SetActive() {
	glUseProgram(_shaderProgram);
}


// ===== ユニフォーム =====//

/**
 * Matrix(行列)ユニフォームを設定する
 * 
 * プログラム内からユニフォームの位置を取得し、行列を設定する
 */
void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix) {

	GLuint loc = glGetUniformLocation(_shaderProgram, name); // 名前を探す

	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr()); //
}

/**
 * Vector4ユニフォームを設定する
 */
void Shader::SetVector4Uniform(const char* name, const Vector4& vec) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
}

/**
 * Vector3ユニフォームを設定する
 */
void Shader::SetVector3Uniform(const char* name, const Vector3& vec) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform3f(loc, vec.x, vec.y, vec.z);
}

/**
 * Vector2ユニフォームを設定する
 */
void Shader::SetVector2Uniform(const char* name, const Vector2& vec) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform2f(loc, vec.x, vec.y);
}

/**
 * Floatユニフォームを設定する
 */
void Shader::SetFloatUniform(const char* name, float value) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform1f(loc, value);
}

/**
 * Int (bool) ユニフォームを設定する
 */
void Shader::SetIntUniform(const char* name, int value) {
	GLuint loc = glGetUniformLocation(_shaderProgram, name);
	glUniform1i(loc, value);
}


//==============================================================================
// プライベート関数
//==============================================================================

/**
 * シェーダーファイルを読み込み、コンパイルする
 * 
 * std::ifstreamを使ってファイルを開き、テキストを読み込む
 * ファイルが開けたら、シェーダータイプを指定し、コンパイルする
 */
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

/**
 * シェーダーがコンパイルされたかどうかを判定する
 * 
 * シェーダーのコンパイル状態を問い合わせ、エラーメッセージを出力する
 */
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

/**
 * シェーダーが有効かどうかを判定する
 *
 * シェーダープログラムのリンク状態を問い合わせ、エラーメッセージを出力する
 */
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
