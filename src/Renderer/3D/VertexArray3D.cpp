#include <GL/glew.h>
#include "LarvaEngine/Renderer/3D/VertexArray3D.h"

//==============================================================================
// �R���X�g���N�^�E�f�X�g���N�^
//==============================================================================

/**
 * ���_�z��I�u�W�F�N�g�A���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬����
 *
 * OpenGL�̒��_�z��I�u�W�F�N�g���쐬����
 * ���b�V���ƃe�N�X�`���̒��_���C�A�E�g���w�肷��
 */
VertexArray3D::VertexArray3D(const float* verts, unsigned int numVerts, unsigned int* indices, unsigned int numIndices)
	: _numVerts(numVerts)
	, _numIndices(numIndices) {

	// ���_�z��I�u�W�F�N�g���쐬
	glGenVertexArrays(1, &_vertexArray); // ���_�z��I�u�W�F�N�g���쐬
	glBindVertexArray(_vertexArray); // ID��ۑ�

	// ���_�o�b�t�@���쐬
	glGenBuffers(1, &_vertexBuffer); // ���_�o�b�t�@���쐬
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer); // ID��ۑ�
	glBufferData(GL_ARRAY_BUFFER, numVerts * 8 * sizeof(float), verts, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@���쐬
	glGenBuffers(1, &_indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// ���_�z��̐ݒ�

	// ���b�V���̒��_���C�A�E�g���w��
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); // �T�C�Y�A��ށA�t�H�[�}�b�g���w��
	// ���b�V���̒��_���C�A�E�g���w��
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 3)); // �T�C�Y�A��ށA�t�H�[�}�b�g���w��
	// �e�N�X�`���̒��_���C�A�E�g���w��
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<void*>(sizeof(float) * 6));
}

/**
 * �f�X�g���N�^
 *
 * ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���폜����
 */
VertexArray3D::~VertexArray3D() {
	glDeleteBuffers(1, &_vertexBuffer);
	glDeleteBuffers(1, &_indexBuffer);
	glDeleteVertexArrays(1, &_vertexArray);
}


//==============================================================================
// �����o�֐�
//==============================================================================

/**
 * ���_�z����A�N�e�B�u�ɂ���
 *
 * ���_�z��I�u�W�F�N�g���A�N�e�B�u�ɂ���
 */
void VertexArray3D::SetActive() {
	glBindVertexArray(_vertexArray);
}

