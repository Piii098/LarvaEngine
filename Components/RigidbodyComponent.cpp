#include "Components/RigidbodyComponent.h"
#include "GameObjects/GameObject.h"
#include "Math.h"
#include "Utilities/Frame.h"
#include "PhysWorld2D.h"
#include "Game.h"

RigidbodyComponent::RigidbodyComponent(GameObject* parent, int updateLayer)
	: Component(parent, updateLayer)
	, _angularSpeed(0.f)
	, _forwardSpeed(0.f)
	, _mass(0.f)
	, _drag(0.f)
	, _sumOfForces(Vector2::Zero)
	, _velocity(Vector2::Zero)
	, _isGravity(false){

}

RigidbodyComponent::~RigidbodyComponent() {

}

#pragma region �p�u���b�N�֐�

void RigidbodyComponent::Update(Frame* frame) {
	const float GRAVITY_CONSTANT = 10000.f;
	if (_isGravity) {
		PhysWorld2D* phys = _parent->GetGame()->GetPhysWorld();
		PhysWorld2D::CollisionInfo outColl;

		// �v���C���[�̑��������肩�牺�����Ƀ��C���L���X�g
		Vector2 rayStart = _parent->Position() + Vector2(0.f, -1.0f);
		LineSegment2D ray(rayStart, rayStart + Vector2(0.f, -15));

		if (phys->SegmentCast(ray, outColl, _parent)) {
			if (outColl._object->Tag() != GameObject::TAG::GROUND) {
				// �n�ʈȊO�ɏՓ˂����ꍇ�A�d�͂�K�p
				float gravityForce = _mass * GRAVITY_CONSTANT * frame->DeltaTime();
				AddForce(Vector2(0, -gravityForce));
			}
		}
		else {
			// �Փ˂��Ȃ��ꍇ�A�d�͂�K�p
			float gravityForce = _mass * GRAVITY_CONSTANT * frame->DeltaTime();
			AddForce(Vector2(0, -gravityForce));
		}
	}

	
	/*�j���[�g���͊w*/
	Vector2 accele; // �����x������o��
	accele.x = _sumOfForces.x / _mass; // f = ma -> a = f / m
	accele.y = _sumOfForces.y / _mass;

	_velocity += accele * frame->DeltaTime();

	_velocity *= (1.0f - _drag * frame->DeltaTime());

	Vector2 pos = _parent->Position();

	//pos += _velocity * frame->DeltaTime();

	//_parent->Position(pos);

	_sumOfForces = Vector2::Zero;

}

void RigidbodyComponent::AddForce(Vector2 force) {
	_sumOfForces += force;
}


#pragma endregion
