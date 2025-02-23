#pragma once

class VertexArray {
public:
	VertexArray(const float* verts, unsigned int numVerts // ���_�̔z��Ɛ�
		, unsigned int* indices, unsigned int numIndices); // �C���f�b�N�X�̔z��Ɛ�
	~VertexArray();

	void SetActive(); // ���_�z����A�N�e�B�u�ɂ���(�`�悷��)

	unsigned int NumIndices() const { return _numIndices; };
	unsigned int NumVerts() const { return _numVerts; };
private:
	unsigned int _numVerts;
	unsigned int _numIndices;
	unsigned int _vertexBuffer;
	unsigned int _indexBuffer;
	unsigned int _vertexArray;
};