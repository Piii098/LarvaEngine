/*
#pragma once
#include "GameObjects/GameObject.h"

class BoxComponent2D;
class RigidbodyComponent;


class StaticObject : public GameObject {
public:
	StaticObject(Game* game);
	~StaticObject() override;

	virtual void InputObject(Input* input);

	virtual void UpdateObject(Frame* frame);
	void PhysUpdate(Frame* frame);

	BoxComponent2D* BoxComp();

private:

	BoxComponent2D* _boxComp;

};
*/