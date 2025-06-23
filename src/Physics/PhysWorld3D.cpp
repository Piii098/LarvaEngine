#include "LarvaEngine/Physics/PhysWorld3D.h"
#include "LarvaEngine/Components/Physics/Box3DComponent.h"
#include "LarvaEngine/Core/Game.h"
#include "LarvaEngine/Core/GameObject.h"
#include "LarvaEngine/Components/Physics/Rigidbody3DComponent.h"
#include <algorithm>

//============================================================
// コンストラクタ・デストラクタ
//============================================================

PhysWorld3D::PhysWorld3D(Game& game)
    : _game(game) {
}

//============================================================
// パブリック関数
//============================================================

bool PhysWorld3D::SegmentCast(const LineSegment3D& l, CollisionInfo3D& outColl, GameObject& ignoreObj) {
    bool collided = false;
    float closestT = std::numeric_limits<float>::infinity();
    Vector3 norm;

    for (auto box : _boxComps) {
        float t;
        if (box->GetParent().IsEqual(ignoreObj)) continue;

        if (Intersect(l, box->GetWorldBox(), t, norm)) {
            if (t < closestT) {
                closestT = t;
                outColl._point = l.PointOnSegment(t);
                outColl._normal = norm;
                outColl._box = box;
                outColl._object = &box->GetParent();
                collided = true;
            }
        }
    }
    return collided;
}

void PhysWorld3D::FixedUpdate(float deltaTime) {
    CollisionUpdate(deltaTime);

    for (auto& rigid : _rigidbodyComps) {
        rigid->CalculateVelocity(deltaTime);
        rigid->UpdatePosition(deltaTime);
    }
}

void PhysWorld3D::AddBoxComponent(Box3DComponent* obj) {
    _boxComps.push_back(obj);
}

void PhysWorld3D::RemoveBoxComponent(Box3DComponent* box) {
    auto iter = std::find(_boxComps.begin(), _boxComps.end(), box);
    if (iter != _boxComps.end()) {
        std::iter_swap(iter, _boxComps.end() - 1);
        _boxComps.pop_back();
    }
}

void PhysWorld3D::AddRigidbodyComponent(Rigidbody3DComponent* obj) {
    _rigidbodyComps.push_back(obj);
}

void PhysWorld3D::RemoveRigidbodyComponent(Rigidbody3DComponent* obj) {
    auto iter = std::find(_rigidbodyComps.begin(), _rigidbodyComps.end(), obj);
    if (iter != _rigidbodyComps.end()) {
        std::iter_swap(iter, _rigidbodyComps.end() - 1);
        _rigidbodyComps.pop_back();
    }
}

//============================================================
// プライベート関数
//============================================================

void PhysWorld3D::CollisionUpdate(float deltaTime) {
    int subSteps = 5;
    float subDeltaTime = deltaTime / subSteps;

    for (int i = 0; i < subSteps; ++i) {
        for (auto boxA : _boxComps) {
            auto rigidA = boxA->GetParent().GetComponent<Rigidbody3DComponent>();
            if (rigidA == nullptr || !boxA->IsCollision() || !boxA->IsDynamic()) {
                continue;
            }

            Vector3 currentPosA = rigidA->InternalPosition();
            Vector3 velocityA = rigidA->Velocity();
            Vector3 nextPosA = currentPosA + velocityA * subDeltaTime;

            AABB3D nextBoxA = boxA->GetWorldBox();
            nextBoxA.MoveCenterTo(nextPosA);

            for (auto boxB : _boxComps) {
                if (boxA == boxB || !boxB->IsCollision()) continue;

                if (Intersect(nextBoxA, boxB->GetWorldBox())) {
                    FixCollision(boxA, velocityA, nextPosA, nextBoxA, boxB);
                }
            }
            rigidA->Velocity(velocityA);
            rigidA->InternalPosition(nextPosA);
        }
    }
}

void PhysWorld3D::FixCollision(Box3DComponent* boxA, Vector3& velocityA, Vector3& nextPosA, AABB3D& nextBoxA, Box3DComponent* boxB) {
    AABB3D dynBox = nextBoxA;
    AABB3D staBox = boxB->GetWorldBox();

    float dx1 = staBox._min.x - dynBox._max.x;
    float dx2 = staBox._max.x - dynBox._min.x;
    float dy1 = staBox._min.y - dynBox._max.y;
    float dy2 = staBox._max.y - dynBox._min.y;
    float dz1 = staBox._min.z - dynBox._max.z;
    float dz2 = staBox._max.z - dynBox._min.z;

    float dx = (std::abs(dx1) < std::abs(dx2)) ? dx1 : dx2;
    float dy = (std::abs(dy1) < std::abs(dy2)) ? dy1 : dy2;
    float dz = (std::abs(dz1) < std::abs(dz2)) ? dz1 : dz2;

    if (std::abs(dx) < std::abs(dy) && std::abs(dx) < std::abs(dz)) {
        nextPosA.x += dx;
        if ((velocityA.x > 0 && dx < 0) || (velocityA.x < 0 && dx > 0)) {
            velocityA.x = 0.f;
        }
    }
    else if (std::abs(dy) < std::abs(dz)) {
        nextPosA.y += dy;
        if ((velocityA.y > 0 && dy < 0) || (velocityA.y < 0 && dy > 0)) {
            velocityA.y = 0.f;
        }
    }
    else {
        nextPosA.z += dz;
        if ((velocityA.z > 0 && dz < 0) || (velocityA.z < 0 && dz > 0)) {
            velocityA.z = 0.f;
        }
    }

    boxA->OnUpdateWorldTransform();
    boxB->OnUpdateWorldTransform();
}