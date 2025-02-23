#pragma once
#include <string>
#include <GL/glew.h>

class Shader {
public:
	Shader();
	~Shader();
	
	bool Load(const std::string& vertName
		, const std::string& fragName);
	void Unload();

	void SetActive();
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetVector3Uniform(const char* name, const Vector3& vector);
	void SetVector2Uniform(const char* name, const Vector2& vector);
	void SetFloatUniform(const char* name, float value);
	void SetIntUniform(const char* name, int value);
	void SetBoolUniform(const char* name, bool flag);

private:
	bool CompileShader(const std::string& fileName
		, GLuint shaderType, GLuint& outShader); // シェーダーファイルを作成、コンパイル
	bool IsCompiled(GLuint shader); // コンパイル成功かを返す
	bool IsValidProgram(); //
	GLuint _vertexShader;
	GLuint _fragShader;
	GLuint _shaderProgram;

};