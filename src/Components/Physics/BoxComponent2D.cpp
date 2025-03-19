#include "LarvaEngine/Components/Physics/BoxComponent2D.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Physics/PhysWorld2D.h"
#include "LarvaEngine/Renderer/Renderer.h"
#include "LarvaEngine/Core/Scene.h"
#include "LarvaEngine/Core/SceneManager.h"

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
