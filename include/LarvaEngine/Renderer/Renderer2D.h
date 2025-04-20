#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include "LarvaEngine/Core/Utilities/Math.h" 

/// @brief �A���r�G���g���C�g�\����
/// �J���[�Ƌ��x�������e���C���[�ɐݒ肷��
struct AmbientLight {

    /// @brief �w��R���X�g���N�^
    /// @param color ���C�g�J���[ 
    /// @param intensity ���x
    AmbientLight(Vector3 color, float intensity) : color(color), intensity(intensity) {}
    
	/// @brief �f�t�H���g�R���X�g���N�^
	/// �f�t�H���g�̃J���[�Ƌ��x��ݒ肷��(�e�����Ȃ�)
	AmbientLight() : color(Vector3(1, 1, 1)), intensity(1.f) {}

	Vector3 color;    ///< �J���[
	float intensity;  ///< ���x
};

// ===== �O���錾 ===== //
class Game;
class VertexArray;
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
class Renderer {
public:

	// ===== �R���X�g���N�^�E�f�X�g���N�^ =====//
    Renderer(Game& game);
    ~Renderer();


	// ===== ���������� =====//

	/// @brief ������
	/// �E�B���h�E���쐬���AOpenGL�̏��������s��
	/// @param screenWidth �E�B���h�E�̕�
	/// @param screenHeight �E�B���h�E�̍���
	/// @param lowResWidth ��𑜓x�����_�����O�̕�
	/// @param lowResHeight ��𑜓x�����_�����O�̍���
    bool Initialize(float screenWidth, float screenHeight, float lowResWidth, float lowResHeight, const std::string& windowName);
    

	/// @brief �`�ʏ���
	/// ���C���V�[����UI�V�[����`�ʂ���
	/// ���C���[���Ƃ̕`�ʂ̂��A���C�g��`�ʂ���
	/// �|�X�g�v���Z�X���s���AUI��`�ʂ���
    void Render();

	
	// ===== ���C���[�֘A ===== //

    /// @brief �p�����b�N�X�W���̐ݒ� 
	/// �W����0�𒆉����C���[�Ƃ��A1���Ŕw�ʃ��C���[�Ƃ���
	/// -�͑O�ʂɁA+�͔w�ʂɈړ�����
    /// @param layer �w�背�C���[ 
	/// @param factor �p�����e�b�N�X�W��
    void SetParallaxFactor(int layer, float factor);

	/// @brief �p�����b�N�X�W���̎擾
	/// @param layer �w�背�C���[
	/// @return �p�����b�N�X�W��
    float GetParallaxFactor(int layer) const;

	/// @brief �������C���[�̐ݒ�
	/// �p�����b�N�X�W���̊�ƂȂ郌�C���[��ݒ肷��
	/// @param layer �������C���[
    void SetCentralLayer(int layer);


	// ===== ���C�g�֘A ===== //

	/// @brief �A���r�G���g���C�g�W���̐ݒ�
	/// �e���C���[�ɃA���r�G���g���C�g��ݒ肷��
	/// @param layer �w�背�C���[
	/// @param factor �A���r�G���g���C�g�W��
	void SetAmbientLightFactor(int layer, AmbientLight factor);

	/// @brief �S�ẴA���r�G���g���C�g�W���̐ݒ�
	/// �e���C���[�ɃA���r�G���g���C�g��ݒ肷��
	/// @param factor �A���r�G���g���C�g�W��
	void SetAllAmbientLightFactor(AmbientLight factor);

	/// @brief �A���r�G���g���C�g�W���̎擾
	/// @param layer �w�背�C���[
	/// @return �A���r�G���g���C�g�W��
	AmbientLight GetAmbientLightFactor(int layer) const;

	/// @brief ���C�g�̒ǉ�
	/// ���C�g�R���|�[�l���g��ǉ�����
	/// @param light ���C�g�R���|�[�l���g
	void AddLight(LightComponent* light);

	/// @brief ���C�g�̍폜
	/// ���C�g�R���|�[�l���g���폜����
	/// @param light ���C�g�R���|�[�l���g
	void RemoveLight(LightComponent* light);

	
	// ===== �Q�b�^�[ ===== //

	SDL_Window* GetWindow() const { return _window; }

private:

	// ===== ���������� ===== //
    
	/// @brief �t���[���o�b�t�@�̏�����
	bool InitializeFrameBuffer();


	// ===== �`�ʏ��� ===== //

	/// @brief ���C���V�[���̕`��
	/// @param view �J�����̃r���[�s��
	/// @param region �`�ʂ��郌�C���[
    void DrawMainScene(MainScene& mainScene, Matrix4 view, int region);
	
	/// @brief ���C�g�̕`��
	/// @param view �J�����̃r���[�s��
	/// @param region �`�ʂ��郌�C���[
	void DrawLight(Matrix4 view, int region);

	/// @brief ���C�g�̌���
	/// @param region �`�ʂ��郌�C���[
	void CombineLight(int region);

	/// @brief �|�X�g�v���Z�X�̕`��
	/// �u���[���G�t�F�N�g���s��
	void PostProcess();

	/// @brief UI������
	/// ���C���V�[����UI�V�[������������
	void DrawAndCombineUI(MainScene& mainScene);

	/// @brief UI�̕`��
	/// UI�V�[����`�ʂ���
	void DrawUI(MainScene& mainScene);

	/// @brief �E�B���h�E�̃X���b�v
	/// �_�u���o�b�t�@�����O���s��
    void SwapWindow();

	
	// ===== ���\�[�X�֘A ===== //

	/// @brief �V�F�[�_�[�̓ǂݍ���
	/// @return ����������true
    bool LoadShaders();

	/// @brief ���_�z��̍쐬
    void CreateVerts();


	// ===== �I������ ===== //

	/// @brief �I������
    void Shutdown();


	// ===== �t���[���o�b�t�@�֘A ===== //

	// ���C���V�[���֘A
	std::vector<unsigned int> _layerFBOs;		///< ���C���[���Ƃ̃t���[���o�b�t�@
	std::vector<unsigned int> _layerBuffers;	///< ���C���[���Ƃ̃e�N�X�`��
	std::vector<unsigned int> _lightFBOs;		///< ���C�g���Ƃ̃t���[���o�b�t�@
	std::vector<unsigned int> _lightBuffers;	///< ���C�g���Ƃ̃e�N�X�`��
	unsigned int _combineLightFBO;				///< ���C�g�����p�t���[���o�b�t�@
	unsigned int _combineLightBuffer;			///< ���C�g�����p�o�b�t�@
	
	// �u���[���G�t�F�N�g�֘A
	unsigned int _extractBrightFBO;				///< �u���C�g�l�X���o�p�t���[���o�b�t�@
	unsigned int _extractBrightBuffers[2];		///< �u���C�g�l�X���o�p�o�b�t�@
	unsigned int _pingpongFBO[2];				///< 	
	unsigned int _pingpongBuffers[2];			///<
	unsigned int _finalBloomFBO;				///< �u���[���G�t�F�N�g�p�t���[���o�b�t�@
	unsigned int _finalBloomBuffer;				///< �u���[���G�t�F�N�g�p�o�b�t�@
	 
	// �ŏI�`�ʊ֘A
	unsigned int _finalFBO;						///< �ŏI�`�ʗp�t���[���o�b�t�@
	unsigned int _finalBuffer;					///< �ŏI�`�ʗp�o�b�t�@
    int _numLayers;

    // �p�����b�N�X�֘A
    std::vector<float> _parallaxFactors;		///< �e���C���[�̎����W�� (0.0=�������C���[�Ɠ����A1.0=�Ŕw��)
    int _centralLayer;							///< �������C���[ (��ƂȂ郌�C���[)

	// �A���r�G���g���C�g�֘A
	std::vector<AmbientLight> _ambientLightFactors;	///< �e���C���[�̃A���r�G���g���C�g�W��
	std::vector<LightComponent*> _lights;

    // ��{�I�ȃ����_�����O���\�[�X
	SDL_Window* _window;							///< �E�B���h�E
	SDL_GLContext _context;							///< OpenGL�R���e�L�X�g
	std::unique_ptr<VertexArray> _frameVerts;			///< �t���[���`�ʗp���_�z��
	std::unique_ptr<VertexArray> _spriteVerts;			///< �X�v���C�g�`�ʗp���_�z��
	std::unique_ptr<Shader> _frameShader;				///< �t���[���`�ʗp�V�F�[�_�[
	std::unique_ptr<Shader> _spriteShader;				///< �X�v���C�g�`�ʗp�V�F�[�_�[
	std::unique_ptr<Shader> _lightShader;				///< ���C�g�`�ʗp�V�F�[�_�[
	std::unique_ptr<Shader> _combineShader;				///< ���C�g�����p�V�F�[�_�[
	std::unique_ptr<Shader> _extractBrightShader;		///< �u���C�g�l�X���o�p�V�F�[�_�[
	std::unique_ptr<Shader> _blurShader;				///< �u���[�p�V�F�[�_�[
	std::unique_ptr<Shader> _finalBloomShader;			///< �u���[���G�t�F�N�g�p�V�F�[�_�[

    // �E�B���h�E�ݒ�
	float _screenWidth;							///< �E�B���h�E�̕�
	float _screenHeight;						///< �E�B���h�E�̍���	
	float _lowResWidth;							///< ��𑜓x�����_�����O�̕�
	float _lowResHeight;						///< ��𑜓x�����_�����O�̍���	

    // �Q�[�����\�[�X
	Game& _game;								///< �Q�[���N���X

};

//�]�n: ���C�g��MainScene�Ɉړ�����(��ѐ��̂���)
//�]�n: �V�F�[�_�[���A�Z�b�g�}�l�[�W���[�ŊǗ�����