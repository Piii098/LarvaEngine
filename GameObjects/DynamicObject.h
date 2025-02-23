#pragma once
#include "GameObjects/GameObject.h"

class BoxComponent2D;
class RigidbodyComponent;


class DynamicObject : public GameObject {
public:
	DynamicObject(Game* game);
	~DynamicObject() override;
	
	//virtual void InputObject(Input* input);

	//virtual void UpdateObject(Frame* frame);
	//void PhysUpdate(Frame* frame);

	//BoxComponent2D* BoxComp();
	//RigidbodyComponent* RigidbodyComp();
private:

	BoxComponent2D* _boxComp;
	RigidbodyComponent* _rigidbodyComp;

	/*�j���[�g���͊w*/
	float _mass; //����
	Vector2 _sumOfForces; //�͂̍��v
	Vector2 _velocity; //���x
};