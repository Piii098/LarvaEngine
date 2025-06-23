#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Utilities/Math.h" 
#include "LarvaEngine/Renderer/Renderer.h"

// ===== �O���錾 ===== //
class VertexArray3D;
class Texture;
class Shader;
class Camera;
class MainScene;
class UIScene;
class LightComponent;


/// @brief �����_���[�N���X
/// �Q�[���̕`����Ǘ�����
/// ���C���[���Ƃɕ`�悷�邱�ƕ`�ʂ���
/// �p�����b�N�X��ݒ肷�邱�ƂŎ�����\������
/// �A���r�G���g���C�g��ݒ肷�邱�ƂŊe���C���[�Ɍ��𓖂Ă�
/// ���C�g�R���|�[�l���g��ǉ����邱�ƂŃ��C�g��ǉ�����
/// ���C���V�[���͒�𑜓x�����_�����O�AUI�V�[���͎��E�B���h�E�����_�����O��UI���d�˂�`�ŕ`�ʂ���
class Renderer3D : public Renderer {
public:

	// ===== �R���X�g���N�^�E�f�X�g���N�^ =====//
	Renderer3D(Game& game);
	~Renderer3D() override;


	// ===== ���������� =====//

	/// @brief ������
	/// �E�B���h�E���쐬���AOpenGL�̏��������s��
	/// @param screenWidth �E�B���h�E�̕�
	/// @param screenHeight �E�B���h�E�̍���
	/// @param lowResWidth ��𑜓x�����_�����O�̕�
	/// @param lowResHeight ��𑜓x�����_�����O�̍���
	bool Initialize(const std::string& windowName) override;

	/// @brief �`�ʏ���
	/// ���C���V�[����UI�V�[����`�ʂ���
	/// ���C���[���Ƃ̕`�ʂ̂��A���C�g��`�ʂ���
	/// �|�X�g�v���Z�X���s���AUI��`�ʂ���
	void Render() override;

	/// @brief ���C�g�̒ǉ�
	/// ���C�g�R���|�[�l���g��ǉ�����
	/// @param light ���C�g�R���|�[�l���g
	void AddLight(LightComponent* light) override {}

	/// @brief ���C�g�̍폜
	/// ���C�g�R���|�[�l���g���폜����
	/// @param light ���C�g�R���|�[�l���g
	void RemoveLight(LightComponent* light) override {}


	// ===== �Q�b�^�[ ===== //

	SDL_Window* GetWindow() const { return _window; }

	void SetLowResResolution(float width, float height) {
		_lowResWidth = width;
		_lowResHeight = height;
	}

private:

	// ===== ���\�[�X�֘A ===== //

	/// @brief �V�F�[�_�[�̓ǂݍ���
	/// @return ����������true
	bool LoadShaders();

	/// @brief ���_�z��̍쐬
	void CreateVerts();

	void SetLightUniforms(Shader& shader);

	void InitShadowMap();
	void InitDOF();

	/// @brief �I������
	void Shutdown();

	std::unique_ptr<Shader> _meshShader; ///< �V�F�[�_�[
	std::unique_ptr<Shader> _spriteShader; ///< ���C�g�V�F�[�_�[
	std::unique_ptr<Shader> _toonShader; ///< �g�D�[���V�F�[�_�[
	std::unique_ptr<Shader> _depthShader; ///< ���C�g�V�F�[�_�[
	std::unique_ptr<Shader> _frameShader;
	std::unique_ptr<Shader> _basicShader; ///< �|�X�g�v���Z�X�V�F�[�_�[
	std::unique_ptr<Shader> _shadowShader; ///< �u���[���V�F�[�_�[
	std::unique_ptr<Shader> _outlineShader; ///< �u���[���V�F�[�_�[
	std::unique_ptr<Shader> _dofShader; ///< DOF�V�F�[�_�[
	std::unique_ptr<Shader> _blurShader; ///< �u���[���V�F�[�_�[

	unsigned int _depthFBO; ///< �V���h�E�}�b�v�̃t���[���o�b�t�@�I�u�W�F�N�g
	unsigned int _depthBuffer; ///< �V���h�E�}�b�v�̃e�N�X�`��

	unsigned int _colorFBO;
	unsigned int _colorBuffer; ///< �J���[�o�b�t�@
	unsigned int _dofDepthBuffer; ///< DOF�̐[�x�o�b�t�@
	unsigned int _colorDepthTexture; ///< DOF�̃t���[���o�b�t�@�I�u�W�F�N�g
	unsigned int _pingpongFBO[2];				///< 	
	unsigned int _pingpongBuffers[2];

	VertexArray3D* _spriteVerts; ///< ���b�V���̒��_�z��
	VertexArray3D* _frameVerts; ///< �X�v���C�g�̒��_�z��

	Matrix4 _view;
	Matrix4 _projection;

	Texture* _toonRampTexture; ///< �g�D�[���V�F�[�_�[�p�̃e�N�X�`��
	
	Vector3 _ambientLight; ///< �A���r�G���g���C�g�̐F

};

//�]�n: ���C�g��MainScene�Ɉړ�����(��ѐ��̂���)
//�]�n: �V�F�[�_�[���A�Z�b�g�}�l�[�W���[�ŊǗ�����