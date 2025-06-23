#pragma once


/// @brief ���_�z��
/// ���_�z��̐ݒ��n���āAOpenGL�̒��_�z��I�u�W�F�N�g���쐬����
class VertexArray3D {
public:

	// ===== �R���X�g���N�^�E�f�X�g���N�^ =====//

	/// @brief �R���X�g���N�^
	/// ���_�z��I�u�W�F�N�g�A���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬����
	/// @param verts ���_�̔z��
	/// @param numVerts ���_�̐�
	/// @param indices �C���f�b�N�X�̔z��
	/// @param numIndices �C���f�b�N�X�̐�
	VertexArray3D(const float* verts, unsigned int numVerts, unsigned int* indices, unsigned int numIndices);
	~VertexArray3D();


	// ===== �����o�֐� =====//

	/// @brief ���_�z����A�N�e�B�u�ɂ���
	/// �`�ʑO�ɌĂяo��
	void SetActive(); // ���_�z����A�N�e�B�u�ɂ���(�`�悷��)


	// ===== �Q�b�^�[ =====//

	unsigned int NumIndices() const { return _numIndices; };
	unsigned int NumVerts() const { return _numVerts; };

private:

	unsigned int _numVerts;			///< ���_�̐�
	unsigned int _numIndices;		///< �C���f�b�N�X�̐�
	unsigned int _vertexBuffer;		///< ���_�o�b�t�@
	unsigned int _indexBuffer;		///< �C���f�b�N�X�o�b�t�@
	unsigned int _vertexArray;		///< ���_�z��

};