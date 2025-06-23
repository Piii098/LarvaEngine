#pragma once
#include <vector>
#include <memory>
#include "LarvaEngine/Core/Utilities/Math.h"
#include "LarvaEngine/Core/Component.h"

// ===== �O���錾 ===== //
class FrameSystem;
class Component;
class Game;
class Scene;
class MainScene;
class InputAction;

namespace Type3D{

    ///@brief �Q�[�����̃I�u�W�F�N�g�̊�{�N���X
    ///
    ///�R���|�[�l���g�������A���͏����A�X�V�������s��
    ///�q�I�u�W�F�N�g�������Ƃ��ł���
    ///DEAD��Ԃ̃I�u�W�F�N�g�͔j�������
    class GameObject {
    public:

        enum class STATE {
            ACTIVE,   ///< �A�N�e�B�u���
            INACTIVE, ///< ��A�N�e�B�u���
            DEAD      ///< �j���\����
        };

        /// @brief �^�O
        /// ���g�p
        enum class TAG {
            NONE,     ///< �^�O�Ȃ�
            PLAYER,   ///< �v���C���[
            ENEMY,    ///< �G
            ITEM,     ///< �A�C�e��
            GROUND,   ///< �n��
            OBSTACLE  ///< ��Q��
        };

        // ===== �R���X�g���N�^�E�f�X�g���N�^ =====�@//

        GameObject(Scene& scene);

        GameObject(GameObject* parent);

        virtual ~GameObject();

        // ===== ���[�v���� ===== //

        /// @brief ���͏���
        /// �R���|�[�l���g�ƓƎ��̓��͏������Ăяo��
        /// �V�[�����Ăяo�����
        /// @param input ���͏��
        void ProcessInput(const InputAction& input);

        /// @brief �X�V����
        /// �R���|�[�l���g�ƓƎ��̓��͏������Ăяo��
        /// �V�[�����Ăяo�����
        /// @param deltaTime �t���[���Ԃ̎���
        void Update(float deltaTime);

        /// @brief �����X�V����
        /// �R���|�[�l���g�ƓƎ��̓��͏������Ăяo��
        /// �V�[�����Ăяo����� 
        /// @param deltaTime �t���[���Ԃ̎���
        void FixedUpdate(float deltaTime);

        void LateUpdate(float deltaTime);

        void Cleanup();


        // ====== ���[���h�ϊ��s��֘A ===== //

        /// @brief ���[���h�ϊ��s��̌v�Z
        /// �ʒu�A�X�P�[���A��]���烏�[���h�ϊ��s����v�Z����
        void ComputeWorldTransform();


        // ===== �R���|�[�l���g�֘A ===== //

        /// @brief �R���|�[�l���g�̍폜
        /// �R���|�[�l���g���폜����
        /// @param component �폜����R���|�[�l���g 
        void RemoveComponent(Component* component);

        /// @brief �R���|�[�l���g�̐���
        /// �R���|�[�l���g�𐶐����A�ǉ�����
        /// @tparam T ��������R���|�[�l���g�̌^
        /// @tparam Args �R���X�g���N�^�ɓn�������̌^
        /// @param args �R���X�g���N�^�ɓn������ ( ��������GameObject�ւ̎Q�Ƃ͎����Őݒ肳��� )
        /// @return ���������R���|�[�l���g�ւ̎Q��( �ꎞ�I�ȎQ�Ƃ𐄏� )
        template<typename T, typename... Args>
        T& CreateComponent(Args&&... args) {
            auto component = std::make_unique<T>(*this, std::forward<Args>(args)...);
            T& compRef = *component; // ���|�C���^�ł͂Ȃ��Q�Ƃ��擾
            _components.emplace_back(std::move(component));
            return compRef;
        }

        /// @brief �R���|�[�l���g�̎擾
        /// �w�肵���^�̃R���|�[�l���g���擾����
        /// @tparam T �R���|�[�l���g�̌^
        /// @return �R���|�[�l���g�̃|�C���^
        template <typename T>
        T* GetComponent() {
            for (auto& comp : _components) {
                if (T* t = dynamic_cast<T*>(comp.get())) { // unique_ptr���琶�|�C���^���擾
                    return t;
                }
            }
            return nullptr;
        }

        void DestroyComponent(Component* component);

        void Destroy();
        void AddChildObject(GameObject* child);
        void RemoveChildObject(GameObject* child);

        bool IsEqual(const GameObject& object) const { return this == &object; } ///< �����Ɠ����I�u�W�F�N�g���ǂ����𔻒肷��


        // ===== �Q�b�^�[�Z�b�^�[ ===== //

        void Position(const Vector2& position) { _position = Vector2Int::ToInteger(position); _recomputeWorldTransform = true; }
        void Position(const Vector2Int& position) { _position = position; _recomputeWorldTransform = true; }
        void Scale(float scale) { _scale = scale; _recomputeWorldTransform = true; }
        void Rotation(float rotation) { _rotation = rotation; _recomputeWorldTransform = true; }
        void Tag(TAG tag) { _tag = tag; }
        void State(STATE state) { _state = state; }
        void SetParent(GameObject* parent) { _parent = parent; };

        const Vector2& GetForward() const { return Vector2(Math::Cos(_rotation), Math::Sin(_rotation)); }
        const TAG Tag() const { return _tag; }
        const STATE State() const { return _state; }
        const float Scale() const { return _scale; }
        const float Rotation() const { return _rotation; }
        const Matrix4& WorldTransform() const { return _worldTransform; }


        Scene& GetScene() { return _scene; }
        MainScene& GetMainScene();

    private:

        // ===== ���[�v���� ===== //

        /// @brief �R���|�[�l���g�̓��͏���
        /// �A�N�e�B�u��Ԃ̃R���|�[�l���g�̓��͏������s��
        /// @param input ���͏��
        void InputComponents(const InputAction& input);

        /// @brief �I�u�W�F�N�g�̓��͏���
        /// ���z�֐��A�I�u�W�F�N�g�Ǝ��̓��͏������s��
        /// @param input 
        virtual void InputObject(const InputAction& input) {};

        /// @brief �R���|�[�l���g�̍X�V����
        /// �A�N�e�B�u��Ԃ̃R���|�[�l���g�̍X�V�������s��
        /// @param deltaTime �t���[���Ԃ̎���
        void UpdateComponents(float deltaTime);

        /// @brief �I�u�W�F�N�g�̍X�V����
        /// ���z�֐��A�I�u�W�F�N�g�Ǝ��̍X�V�������s��
        /// @param deltaTime �t���[���Ԃ̎���
        virtual void UpdateObject(float deltaTime) {};

        /// @brief �R���|�[�l���g�̕����X�V����
        /// �A�N�e�B�u��Ԃ̃R���|�[�l���g�̕����X�V�������s��
        /// @param deltaTime �t���[���Ԃ̎���
        void FixedUpdateComponents(float deltaTime);

        /// @brief �I�u�W�F�N�g�̕����X�V����
        /// ���z�֐��A�I�u�W�F�N�g�Ǝ��̕����X�V�������s��
        /// @param deltaTime �t���[���Ԃ̎���
        virtual void FixedUpdateObject(float deltaTime) {};

        void LateUpdateConponents(float deltaTime);

        virtual void LateUpdateObject(float deltaTime) {};


        // ===== �I�u�W�F�N�g�֘A ===== //

        /// @brief �R���|�[�l���g�̒ǉ�
        /// UpdateLayer�̒l�����������ɑ}������
        /// @param component �ǉ�����R���|�[�l���g
        void AddComponent(std::unique_ptr<Component> component);


        // ===== �����o�ϐ� ===== //

        // �V�[���֘A
        Scene& _scene;                              ///< ��������V�[��
        GameObject* _parent;                        ///< �e�I�u�W�F�N�g

        // ��Ԋ֘A
        STATE _state;                               ///< �I�u�W�F�N�g�̏��
        TAG _tag;                                   ///< �I�u�W�F�N�g�̃^�O

        // �R���|�[�l���g
        std::vector <std::unique_ptr<Component>> _components;        ///< �R���|�[�l���g���X�g
        std::vector <GameObject*> _children;          ///< �q�I�u�W�F�N�g���X�g

        // �g�����X�t�H�[���֘A
        Matrix4 _worldTransform;                    ///< ���[���h�ϊ��s��
        bool _recomputeWorldTransform;              ///< �ϊ��s��̍Čv�Z�t���O
        Vector3 _position;                       ///< �ʒu�i�����j
        float _scale;                               ///< �X�P�[��
        float _rotation;                            ///< ��]�i���W�A���j
    };

}