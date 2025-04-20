#pragma once

/// @brief 頂点配列
/// 頂点配列の設定を渡して、OpenGLの頂点配列オブジェクトを作成する
class VertexArray {
public:

	// ===== コンストラクタ・デストラクタ =====//

	/// @brief コンストラクタ
	/// 頂点配列オブジェクト、頂点バッファとインデックスバッファを作成する
	/// @param verts 頂点の配列
	/// @param numVerts 頂点の数
	/// @param indices インデックスの配列
	/// @param numIndices インデックスの数
	VertexArray(const float* verts, unsigned int numVerts , unsigned int* indices, unsigned int numIndices); 
	~VertexArray();


	// ===== メンバ関数 =====//

	/// @brief 頂点配列をアクティブにする
	/// 描写前に呼び出す
	void SetActive(); // 頂点配列をアクティブにする(描画する)


	// ===== ゲッター =====//

	unsigned int NumIndices() const { return _numIndices; };
	unsigned int NumVerts() const { return _numVerts; };

private:

	unsigned int _numVerts;			///< 頂点の数
	unsigned int _numIndices;		///< インデックスの数
	unsigned int _vertexBuffer;		///< 頂点バッファ
	unsigned int _indexBuffer;		///< インデックスバッファ
	unsigned int _vertexArray;		///< 頂点配列

};