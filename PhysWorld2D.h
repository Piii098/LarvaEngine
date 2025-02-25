#pragma once
#include <vector>
#include <functional>
#include "Utilities/Collision2D.h"
#include "Math.h"

class Frame;
class Game;
class GameObject;
class DynamicObject;
class StaticObject;
class BoxComponent2D;
class RigidbodyComponent;

struct DynamicComponent {
	DynamicComponent(GameObject* parent, bool isCollision);
	~DynamicComponent();
	RigidbodyComponent* rigidbodyComp;
	BoxComponent2D* boxComp;
	GameObject* _parent;
};

class PhysWorld2D {
public:

	struct CollisionInfo {
		Vector2 _point;
		Vector2 _normal;
		BoxComponent2D* _box;
		GameObject* _object;
	};

	PhysWorld2D(Game* game);

	bool SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, const GameObject* ignoreObj = nullptr);

	void Update(Frame* frame);

	void CollisionUpdate(Frame* frame);
	void RigidbodyUpdate(Frame* frame);

	void AddDynamicComp(DynamicComponent* obj);
	void AddStaticBoxComp(BoxComponent2D* obj);
	void RemoveDynamicComp(DynamicComponent* obj);
	void RemoveStaticBoxComp(BoxComponent2D* obj);

private:

	void FixCollision(DynamicComponent* dynamicComp, Vector2& velocity, Vector2& nextPos, AABB2D nextBox, BoxComponent2D* staticBox);
	void FixCollision(DynamicComponent* dynamicComp, BoxComponent2D* staticBoxComp);
	
	Game* _game;
	std::vector<DynamicComponent*> _dynamicComps;
	std::vector<BoxComponent2D*> _staticBoxes;
};