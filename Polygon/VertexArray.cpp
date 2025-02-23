#include "Polygon/VertexArray.h"
#include <GL/glew.h>

#pragma region �R���X�g���N�^:�f�X�g���N�^

VertexArray::VertexArray(const float* verts, unsigned int numVerts
	, unsigned int* indices, unsigned int numIndices)
	: _numVerts(numVerts)
	, _numIndices(numIndices) {

	/*���_�z��쐬*/
	glGenVertexArrays(1, &_vertexArray); // ���_�z��I�u�W�F�N�g���쐬
	glBindVertexArray(_vertexArray); // ID��ۑ�

	/*���_�o�b�t�@�쐬*/
	glGenBuffers(1, &_vertexBuffer); // ���_�o�b�t�@���쐬
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer); // ID��ۑ�
	glBufferData(GL_ARRAY_BUFFER, numVerts * 5 * sizeof(float), verts, GL_STATIC_DRAW);

	/*�C���f�b�N�X�o�b�t�@�쐬*/
	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	/*���_���C�A�E�g���w��*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0); // �T�C�Y�A��ށA�t�H�[�}�b�g���w��
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 3));
}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_indexBuffer);
	glDeleteVertexArrays(1, &_vertexArray);
}

#pragma endregion

#pragma region �p�u���b�N�֐�

void VertexArray::SetActive() {
	glBindVertexArray(_vertexArray);
}
#pragma endregion
