#include <algorithm>
#include "PhysWorld2D.h"
#include "Components/BoxComponent2D.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Frame.h"
#include "Components/RigidbodyComponent.h"

#pragma region コンストラクタ:デストラクタ

DynamicComponent::DynamicComponent(GameObject* parent, bool isCollision) {
    rigidbodyComp = new RigidbodyComponent(parent);
    boxComp = new BoxComponent2D(parent, isCollision, true);

    parent->GetGame()->GetPhysWorld()->AddDynamicComp(this);
}

DynamicComponent::~DynamicComponent() {
    delete rigidbodyComp;
    rigidbodyComp = nullptr;
    delete boxComp;
    boxComp = nullptr;
}

PhysWorld2D::PhysWorld2D(Game* game) {

}

#pragma endregion

#pragma region パブリック関数

bool PhysWorld2D::SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, const GameObject* ignoreObj)
{
    bool collided = false;
    float closestT = std::numeric_limits<float>::infinity();  // 安全なInfinity定義
    Vector2 norm;

    // 動的オブジェクトとの交差判定
    for (auto dynaComp : _dynamicComps)
    {
        auto box = dynaComp->boxComp;
        float t;
        // 自分自身のオブジェクトを無視
        if (box->GetParent() == ignoreObj) continue;

        if (Intersect(l, box->GetWorldBox(), t, norm))
        {
            if (t < closestT)
            {
                closestT = t;
                outColl._point = l.PointOnSegment(t);
                outColl._normal = norm;
                outColl._box = box;
                outColl._object = box->GetParent();
                collided = true;
            }
        }
    }

    // 静的オブジェクトとの交差判定
    for (auto box : _staticBoxes)
    {
        float t;
        if (box->GetParent() == ignoreObj) continue;  // 自分自身を無視

        if (Intersect(l, box->GetWorldBox(), t, norm))
        {
            if (t < closestT)
            {
                closestT = t;
                outColl._point = l.PointOnSegment(t);
                outColl._normal = norm;
                outColl._box = box;
                outColl._object = box->GetParent();
                collided = true;
            }
        }
    }
    return collided;
}



void PhysWorld2D::Update(Frame* frame) {
   
    CollisionUpdate(frame);
     // 他の物理更新処理
}


void PhysWorld2D::TestSweepAndPrune(std::function<void(GameObject*, GameObject*)> f)
{
    // Sort by min.x
    std::sort(_boxComps.begin(), _boxComps.end(),
        [](BoxComponent2D* a, BoxComponent2D* b) {
            return a->GetWorldBox()._min.x <
                b->GetWorldBox()._min.x;
        });

    for (size_t i = 0; i < _boxComps.size(); i++)
    {
        // Get max.x for current box
        BoxComponent2D* a = _boxComps[i];
        float max = a->GetWorldBox()._max.x;
        for (size_t j = i + 1; j < _boxComps.size(); j++)
        {
            BoxComponent2D* b = _boxComps[j];
            // If AABB[j] min is past the max bounds of AABB[i],
            // then there aren't any other possible intersections
            // against AABB[i]
            if (b->GetWorldBox()._min.x > max)
            {
                break;
            }
            else if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
            {
                f(a->GetParent(), b->GetParent());
            }
        }
    }
}


void PhysWorld2D::AddBox(BoxComponent2D* box)
{
    _boxComps.emplace_back(box);
}

void PhysWorld2D::RemoveBox(BoxComponent2D* box)
{
    auto iter = std::find(_boxComps.begin(), _boxComps.end(), box);
    if (iter != _boxComps.end())
    {
        std::iter_swap(iter, _boxComps.end() - 1);
        _boxComps.pop_back();
    }
}

void PhysWorld2D::AddDynamicComp(DynamicComponent* obj) {
    _dynamicComps.push_back(obj);
}

void PhysWorld2D::AddStaticBoxComp(BoxComponent2D* obj) {
    _staticBoxes.push_back(obj);
}


#pragma endregion

#pragma region プライベート関数

void PhysWorld2D::CollisionUpdate(Frame* frame) {
    const int subSteps = 5; // サブステップの数
    for (auto compA : _dynamicComps) {
        auto boxA = compA->boxComp;

        Vector2 velocity = compA->rigidbodyComp->Velocity();

        const float velocityThreshold = 0.01f;
        if (Math::Abs(velocity.x) < velocityThreshold && Math::Abs(velocity.y) < velocityThreshold) {
            velocity.x = 0.f;
            continue;
        }

        if (!boxA->IsCollision()) continue;

        for (auto boxB : _staticBoxes) {
            if (!boxB->IsCollision() || boxA == boxB) continue;

            if (Intersect(boxA->GetWorldBox(), boxB->GetWorldBox())) {
                FixCollision(compA, boxB);
            }

        }

        for (auto compB : _dynamicComps) {
            auto boxB = compB->boxComp;

            if (!boxB->IsCollision() || boxA == boxB) continue;

            if (Intersect(boxA->GetWorldBox(), boxB->GetWorldBox())) {
                FixCollision(compA, boxB);
            }

        }

    }
}

void PhysWorld2D::FixCollision(DynamicComponent* dynamicComp, BoxComponent2D* staticBoxComp) {
    dynamicComp->boxComp->OnUpdateWorldTransform();
    AABB2D dynBox = dynamicComp->boxComp->GetWorldBox();
    AABB2D staBox = staticBoxComp->GetWorldBox();

    // 浸入量を計算
    float dx1 = staBox._min.x - dynBox._max.x;  // 左側からの浸入量
    float dx2 = staBox._max.x - dynBox._min.x;  // 右側からの浸入量
    float dy1 = staBox._min.y - dynBox._max.y;  // 下側からの浸入量
    float dy2 = staBox._max.y - dynBox._min.y;  // 上側からの浸入量

    // X方向の最小浸入量を選択
    float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
    // Y方向の最小浸入量を選択
    float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

    Vector2 pos = dynamicComp->boxComp->GetParent()->Position();
    Vector2 velocity = dynamicComp->rigidbodyComp->Velocity();
    // 浸入量が小さい方の軸で補正
    if (Math::Abs(dx) <= Math::Abs(dy)) {
        pos.x += dx;  // X方向補正
        velocity.x = 0.f;
    }
    else {
        pos.y += dy;  // Y方向補正
        velocity.y = 0.f;
    }

    dynamicComp->boxComp->GetParent()->Position(pos);
    dynamicComp->rigidbodyComp->Velocity(velocity);
    dynamicComp->boxComp->OnUpdateWorldTransform();
}


#pragma endregion
