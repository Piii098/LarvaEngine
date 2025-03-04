#pragma once

class VertexArray {
public:
	VertexArray(const float* verts, unsigned int numVerts // 頂点の配列と数
		, unsigned int* indices, unsigned int numIndices); // インデックスの配列と数
	~VertexArray();

	void SetActive(); // 頂点配列をアクティブにする(描画する)

	unsigned int NumIndices() const { return _numIndices; };
	unsigned int NumVerts() const { return _numVerts; };
private:
	unsigned int _numVerts;
	unsigned int _numIndices;
	unsigned int _vertexBuffer;
	unsigned int _indexBuffer;
	unsigned int _vertexArray;

};