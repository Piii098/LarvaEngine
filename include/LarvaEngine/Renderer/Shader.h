#pragma once
#include <string>
#include <GL/glew.h>
#include "LarvaEngine/Core/Utilities/Math.h"

/// @brief シェーダークラス
///
/// シェーダーファイルを読み込み、コンパイルする
/// シェーダーとしてユニフォームの設定、アクティブ化を行う
class Shader {
public:

	// ===== コンストラクタ・デストラクタ =====//

	Shader();
	~Shader();
	

	// ===== ロード・アンロード =====//

	/// @brief シェーダーファイルを読み込む
	/// 二つのシェーダーファイルを読み込み、コンパイルする
	/// @param vertName 頂点シェーダーファイル名
	/// @param fragName フラグメントシェーダーファイル名
	/// @return 成功したらtrue
	bool Load(const std::string& vertName, const std::string& fragName);
	
	/// @brief シェーダーファイルをアンロードする
	/// シェーダーファイルをアンロードする
	void Unload();


	// ===== アクティブ化 =====//

	/// @brief シェーダープログラムをアクティブにする
	/// 描写前に呼び出す
	void SetActive();
	

	// ===== ユニフォーム =====//

	/// @brief Matrix(行列)ユニフォームを設定する
	/// @param name ユニフォーム名
	/// @param matrix Matrix4(行列)
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	
	void SetVector4Uniform(const char* name, const Vector4& vector);

	/// @brief Vector3ユニフォームを設定する
	/// @param name ユニフォーム名
	/// @param vector Vector3
	void SetVector3Uniform(const char* name, const Vector3& vector);

	/// @brief Vector2ユニフォームを設定する
	/// @param name ユニフォーム名
	/// @param vector Vector2
	void SetVector2Uniform(const char* name, const Vector2& vector);

	/// @brief Floatユニフォームを設定する
	/// @param name ユニフォーム名
	/// @param value Float値
	void SetFloatUniform(const char* name, float value);
	
	/// @brief Int (bool) ユニフォームを設定する
	/// @param name ユニフォーム名
	/// @param value Int値
	void SetIntUniform(const char* name, int value);

private:

	// ===== コンパイル関連 =====//

	/// @brief シェーダーファイルを作成、コンパイルする
	/// @param fileName シェーダーファイル名
	/// @param shaderType シェーダータイプ
	/// @param outShader 作成したシェーダー
	/// @return 成功したらtrue
	bool CompileShader(const std::string& fileName, GLuint shaderType, GLuint& outShader); // シェーダーファイルを作成、コンパイル
	
	/// @brief シェーダープログラムをリンクする
	/// @return 成功したらtrue
	/// @remark リンクに失敗した場合、エラーログを出力する
	bool IsCompiled(GLuint shader); // コンパイル成功かを返す

	/// @brief シェーダーが有効かどうかを判定する
	/// @return 有効であればtrue
	bool IsValidProgram(); //


	GLuint _vertexShader;	///< 頂点シェーダー
	GLuint _fragShader;		///< フラグメントシェーダー
	GLuint _shaderProgram;  ///< シェーダープログラム

};

//余地: アセットとして処理する