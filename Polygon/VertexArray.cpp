#include "Polygon/VertexArray.h"
#include <GL/glew.h>

#pragma region コンストラクタ:デストラクタ

VertexArray::VertexArray(const float* verts, unsigned int numVerts
	, unsigned int* indices, unsigned int numIndices)
	: _numVerts(numVerts)
	, _numIndices(numIndices) {

	/*頂点配列作成*/
	glGenVertexArrays(1, &_vertexArray); // 頂点配列オブジェクトを作成
	glBindVertexArray(_vertexArray); // IDを保存

	/*頂点バッファ作成*/
	glGenBuffers(1, &_vertexBuffer); // 頂点バッファを作成
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer); // IDを保存
	glBufferData(GL_ARRAY_BUFFER, numVerts * 5 * sizeof(float), verts, GL_STATIC_DRAW);

	/*インデックスバッファ作成*/
	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	/*頂点レイアウトを指定*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); // サイズ、種類、フォーマットを指定
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_indexBuffer);
	glDeleteVertexArrays(1, &_vertexArray);
}

#pragma endregion

#pragma region パブリック関数

void VertexArray::SetActive() {
	glBindVertexArray(_vertexArray);
}
#pragma endregion
