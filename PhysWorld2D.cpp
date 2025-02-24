#include <algorithm>
#include "PhysWorld2D.h"
#include "Components/BoxComponent2D.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObjects/GameObject.h"
#include "Utilities/Frame.h"
#include "Components/RigidbodyComponent.h"

#pragma region �R���X�g���N�^:�f�X�g���N�^

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

#pragma region �p�u���b�N�֐�

bool PhysWorld2D::SegmentCast(const LineSegment2D& l, CollisionInfo& outColl, const GameObject* ignoreObj)
{
    bool collided = false;
    float closestT = std::numeric_limits<float>::infinity();  // ���S��Infinity��`
    Vector2 norm;

    // ���I�I�u�W�F�N�g�Ƃ̌�������
    for (auto dynaComp : _dynamicComps)
    {
        auto box = dynaComp->boxComp;
        float t;
        // �������g�̃I�u�W�F�N�g�𖳎�
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

    // �ÓI�I�u�W�F�N�g�Ƃ̌�������
    for (auto box : _staticBoxes)
    {
        float t;
        if (box->GetParent() == ignoreObj) continue;  // �������g�𖳎�

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
     // ���̕����X�V����
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

#pragma region �v���C�x�[�g�֐�

void PhysWorld2D::CollisionUpdate(Frame* frame) {
    const int subSteps = 5; // �T�u�X�e�b�v�̐�
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

    // �Z���ʂ��v�Z
    float dx1 = staBox._min.x - dynBox._max.x;  // ��������̐Z����
    float dx2 = staBox._max.x - dynBox._min.x;  // �E������̐Z����
    float dy1 = staBox._min.y - dynBox._max.y;  // ��������̐Z����
    float dy2 = staBox._max.y - dynBox._min.y;  // �㑤����̐Z����

    // X�����̍ŏ��Z���ʂ�I��
    float dx = (Math::Abs(dx1) < Math::Abs(dx2)) ? dx1 : dx2;
    // Y�����̍ŏ��Z���ʂ�I��
    float dy = (Math::Abs(dy1) < Math::Abs(dy2)) ? dy1 : dy2;

    Vector2 pos = dynamicComp->boxComp->GetParent()->Position();
    Vector2 velocity = dynamicComp->rigidbodyComp->Velocity();
    // �Z���ʂ����������̎��ŕ␳
    if (Math::Abs(dx) <= Math::Abs(dy)) {
        pos.x += dx;  // X�����␳
        velocity.x = 0.f;
    }
    else {
        pos.y += dy;  // Y�����␳
        velocity.y = 0.f;
    }

    dynamicComp->boxComp->GetParent()->Position(pos);
    dynamicComp->rigidbodyComp->Velocity(velocity);
    dynamicComp->boxComp->OnUpdateWorldTransform();
}


#pragma endregion
