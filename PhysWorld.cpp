#include "PhysWorld.h"

#pragma region コンストラクタ:デストラクタ

PhysWorld::PhysWorld(Game* game) {

}

#pragma endregion

#pragma region パブリック関数

bool PhysWorld2D::SegmentCast(const LineSegment2D& l, CollisionInfo& outColl)
{
    bool collided = false;
    float closestT = Math::Infinity;
    Vector2 norm;

    for (auto box : mBoxes)
    {
        float t;
        if (Intersect(l, box->GetWorldBox(), t, norm))
        {
            if (t < closestT)
            {
                closestT = t;
                outColl.mPoint = l.PointOnSegment(t);
                outColl.mNormal = norm;
                outColl.mBox = box;
                outColl.mObject = box->GetOwner();
                collided = true;
            }
        }
    }
    return collided;
}

#pragma endregion
