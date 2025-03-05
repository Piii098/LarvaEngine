#include "Components/Phys/BoxComponent2D.h"
#include "GameObjects/GameObject.h"
#include "Core/Game.h"
#include "Phys/PhysWorld2D.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#pragma region コンストラクタ:デストラクタ

BoxComponent2D::BoxComponent2D(GameObject* parent, bool isCollision, bool isDynamic, int updateLayer)
	: Component(parent)
	, _objectBox(Vector2Int::Zero, Vector2Int::Zero)
	, _worldBox(Vector2Int::Zero, Vector2Int::Zero)
	, _isCollision(isCollision)
	, _isDynamic(isDynamic){

	_parent->GetScene()->GetManager()->GetGame()->GetPhysWorld()->AddBoxComp(this);

}

BoxComponent2D::~BoxComponent2D() {
	_parent->GetScene()->GetManager()->GetGame()->GetPhysWorld()->RemoveBoxComp(this);
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
