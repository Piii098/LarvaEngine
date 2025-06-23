#pragma once
#include <vector>

class BoxComponent; // �O���錾
class RigidbodyComponent; // �O���錾
class Game; // �O���錾

class PhysWorld {
public:
	PhysWorld(Game& game);
	virtual ~PhysWorld();

	virtual void FixedUpdate(float deltaTime);

	void AddBoxComponent(BoxComponent* obj); // �ÓI�{�b�N�X�R���|�[�l���g��ǉ�
	void RemoveBoxComponent(BoxComponent* obj); // �ÓI�{�b�N�X�R���|�[�l���g���폜

	void AddRigidbodyComponent(RigidbodyComponent* obj); // ���I���W�b�h�{�f�B�R���|�[�l���g��ǉ�
	void RemoveRigidbodyComponent(RigidbodyComponent* obj); // ���I���W�b�h�{�f�B�R���|�[�l���g���폜

protected:

	Game& _game; ///< �Q�[���N���X

	std::vector<BoxComponent*> _boxComps; ///< �{�b�N�X�R���|�[�l���g�̔z��
	std::vector<RigidbodyComponent*> _rigidbodyComps; ///< ���W�b�h�{�f�B�R���|�[�l���g�̔z��

};