#pragma once
#include <vector>
#include <functional>
#include "LarvaEngine/Physics/Collision3D.h"
#include "LarvaEngine/Core/Utilities/Math.h"

// ===== �O���錾 ===== //
class Game;
class GameObject;
class Box3DComponent;
class Rigidbody3DComponent;

// �Փˏ��
struct CollisionInfo3D {
    Vector3 _point;         ///< �Փ˓_
    Vector3 _normal;        ///< �Փ˖@��
    Box3DComponent* _box;   ///< �Փ˂����{�b�N�X�R���|�[�l���g
    GameObject* _object;    ///< �Փ˂����I�u�W�F�N�g
};

/// @brief 3D�̕������E�N���X
///
/// 3D�̕������E���Ǘ�����
/// �ÓI�ȃ{�b�N�X�R���|�[�l���g�Ɠ��I�ȃ��W�b�h�{�f�B�R���|�[�l���g���Ǘ�����
/// �Փ˔���A�ՓˏC�����s��
class PhysWorld3D {
public:
    // ===== �R���X�g���N�^ =====//
    PhysWorld3D(Game& game);

    // ===== �Փ˔��� =====//
    /// @brief �����Ƃ̏Փ˔���
    /// ���C�L���X�g�Ƃ��Ďg�p
    bool SegmentCast(const LineSegment3D& l, CollisionInfo3D& outColl, GameObject& ignoreObj);

    // ===== �X�V���� =====//
    /// @brief �X�V����
    void FixedUpdate(float deltaTime);

    // ===== �{�b�N�X�R���|�[�l���g�֘A =====//
    void AddBoxComponent(Box3DComponent* obj);
    void RemoveBoxComponent(Box3DComponent* obj);
    void AddRigidbodyComponent(Rigidbody3DComponent* obj);
    void RemoveRigidbodyComponent(Rigidbody3DComponent* obj);

private:
    // ===== �Փ˔��� =====//
    void CollisionUpdate(float deltaTime);
    void FixCollision(Box3DComponent* boxA, Vector3& velocityA, Vector3& nextPositionA, AABB3D& nextBoxA, Box3DComponent* boxB);

    Game& _game;                                ///< �Q�[���N���X
    const float CCD_STEP_SIZE = 0.01f;          ///< CCD�̃X�e�b�v�T�C�Y
    std::vector<Box3DComponent*> _boxComps;    ///< �{�b�N�X�R���|�[�l���g�̔z��
    std::vector<Rigidbody3DComponent*> _rigidbodyComps; ///< ���W�b�h�{�f�B�R���|�[�l���g�̔z��
};