#include "Components/BoxComponent2D.h"
#include "GameObjects/GameObject.h"
#include "Game.h"
#include "PhysWorld2D.h"
#include "Renderer.h"

#pragma region コンストラクタ:デストラクタ

BoxComponent2D::BoxComponent2D(GameObject* parent, bool isCollision, bool isDynamic, int updateLayer)
	: Component(parent)
	, _objectBox(Vector2::Zero,Vector2::Zero)
	, _worldBox(Vector2::Zero, Vector2::Zero) 
	, _isCollision(isCollision)
	, _isDynamic(isDynamic){

	if (!isDynamic) {
		_parent->GetGame()->GetPhysWorld()->AddStaticBoxComp(this);
	}

}

BoxComponent2D::~BoxComponent2D() {
	_parent->GetGame()->GetPhysWorld()->RemoveBox(this);
}

#pragma endregion

#pragma region パブリック関数

void BoxComponent2D::OnUpdateWorldTransform() {

	_worldBox = _objectBox;

	_worldBox._min *= _parent->Scale();
	_worldBox._max *= _parent->Scale();
	
	_worldBox._min += _parent->Position();
	_worldBox._max += _parent->Position();
}


#pragma endregion
