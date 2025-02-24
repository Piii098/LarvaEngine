#pragma once
#include <vector>
#include <functional>
#include "Utilities/Collision2D.h"
#include "Math.h"

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

	void Update();

	void CollisionUpdate();

	void TestSweepAndPrune(std::function<void(GameObject*, GameObject*)> f);

	void AddBox(BoxComponent2D* box);
	void RemoveBox(BoxComponent2D* box);
	void AddDynamicComp(DynamicComponent* obj);
	void AddStaticBoxComp(BoxComponent2D* obj);

private:
	void FixCollision(DynamicComponent* dynamicComp,BoxComponent2D* staticBox);

	
	Game* _game;
	std::vector<BoxComponent2D*> _boxComps;

	std::vector<DynamicComponent*> _dynamicComps;
	std::vector<BoxComponent2D*> _staticBoxes;
};