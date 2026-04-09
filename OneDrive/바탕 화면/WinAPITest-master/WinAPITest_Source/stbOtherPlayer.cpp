#include "stbOtherPlayer.h"
#include "stbTransform.h"
#include "stbAnimator.h"
#include "stbResourceManager.h"
#include "stbTexture.h"
#include "stbTime.h"
#include "stbD2DRenderer.h"

#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()
#define M_Time stb::SingletonBase<stb::Time>::getInstance()

namespace stb
{
    OtherPlayer::OtherPlayer()
        : mCharacterId("")
        , mTargetPosition(Vector2::Zero)
        , mHasTarget(false)
        , mInterpolationSpeed(800.0f) // 500 -> 800으로 증가 (더 빠르게 따라감)
    {
    }

    OtherPlayer::~OtherPlayer()
    {
    }

    void OtherPlayer::Initialize()
    {
        GameObject::Initialize();
    }

    void OtherPlayer::Update()
    {
        GameObject::Update();
        
        // 목표 위치가 있으면 부드럽게 이동
        if (mHasTarget)
        {
            Transform* tr = GetComponent<Transform>();
            if (tr)
            {
                Vector2 currentPos = tr->GetPosition();
                Vector2 direction = mTargetPosition - currentPos;
                float distance = direction.length();
                
                if (distance > 0.5f) // 목표에 거의 도달하지 않았으면
                {
                    // Lerp 방식: 거리에 비례해서 부드럽게 이동
                    float lerpFactor = 10.0f * M_Time->GetDeltaTime(); // 초당 10배 속도로 따라감
                    if (lerpFactor > 1.0f) lerpFactor = 1.0f;
                    
                    Vector2 newPos = currentPos + direction * lerpFactor;
                    tr->SetPosition(newPos);
                    SyncFollowers(newPos);
                }
                else
                {
                    // 목표에 도달
                    tr->SetPosition(mTargetPosition);
                    SyncFollowers(mTargetPosition);
                    mHasTarget = false;
                }
            }
        }
    }

    void OtherPlayer::LateUpdate()
    {
        GameObject::LateUpdate();
    }

    void OtherPlayer::Render(HDC hdc)
    {
        GameObject::Render(hdc);
    }

    void OtherPlayer::Render(stbD2DRenderer& renderer)
    {
        GameObject::Render(renderer);
    }


    void OtherPlayer::UpdatePosition(float x, float y)
    {
        Transform* tr = GetComponent<Transform>();
        if (tr)
        {
            tr->SetPosition(Vector2(x, y));
        }
    }

    void OtherPlayer::SetTargetPosition(float x, float y, float speed)
    {
        mTargetPosition = Vector2(x, y);
        mTargetSpeed = speed;
        mHasTarget = true;
    }

    void OtherPlayer::SyncFollowers(Vector2 pos)
    {
        for (auto& f : mFollowers)
        {
            if (f.obj)
            {
                Transform* tr = f.obj->GetComponent<Transform>();
                if (tr) tr->SetPosition(Vector2(pos.x + f.offset.x, pos.y + f.offset.y));
            }
        }
    }
}
