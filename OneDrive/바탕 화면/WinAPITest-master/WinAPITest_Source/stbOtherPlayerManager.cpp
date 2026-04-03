#include "stbOtherPlayerManager.h"
#include "stbObject.h"
#include "stbTransform.h"
#include "stbAnimator.h"
#include "stbSpriteRenderer.h"
#include "stbResourceManager.h"
#include "stbTexture.h"
#include "stbEnums.h"

#define M_REMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()

namespace stb
{
    OtherPlayerManager::OtherPlayerManager()
    {
    }

    OtherPlayerManager::~OtherPlayerManager()
    {
        Clear();
    }

    void OtherPlayerManager::UpdatePlayer(const std::string& charId, float x, float y)
    {
        // 이미 존재하는 플레이어인지 확인
        auto it = mPlayers.find(charId);
        
        if (it != mPlayers.end())
        {
            // 기존 플레이어 위치 업데이트
            if (it->second != nullptr)
            {
                it->second->UpdatePosition(x, y);
            }
        }
        else
        {
            // 새로운 플레이어 생성
            OtherPlayer* player = object::Instantiate<OtherPlayer>(enums::eLayerType::Player, Vector2(x, y));
            if (player == nullptr)
            {
                OutputDebugStringA("플레이어 생성 실패!\n");
                return;
            }

            player->SetCharacterId(charId);
            
            // Transform 설정
            Transform* tr = player->GetComponent<Transform>();
            if (tr != nullptr)
            {
                tr->SetPosition(Vector2(x, y));
            }
            
            // DamonKnight 텍스처 사용
            Texture* knightTex = M_REMANAGER->Find<Texture>(L"DamonKnight");
            if (knightTex != nullptr)
            {
                Animator* animator = player->AddComponent<Animator>();
                if (animator != nullptr)
                {
                    animator->CreateAnimation(L"OtherRun", knightTex, Vector2(0.0f, 0.0f), Vector2(67.0f, 81.0f), Vector2::Zero, 3, 0.3f);
                    animator->PlayAnimation(L"OtherRun", true);
                }
            }

            // Sword
            Texture* swordTex = M_REMANAGER->Find<Texture>(L"TwoHandSword");
            if (swordTex != nullptr)
            {
                GameObject* swordObj = object::Instantiate<GameObject>(enums::eLayerType::Player, Vector2(x - 15.0f, y + 7.0f));
                Animator* swordAnim = swordObj->AddComponent<Animator>();
                swordAnim->CreateAnimation(L"OtherRun", swordTex, Vector2(0.0f, 0.0f), Vector2(110.0f, 96.0f), Vector2(-21.5f, -9.0f), 3, 0.3f);
                swordAnim->PlayAnimation(L"OtherRun", true);
                player->AddFollower(swordObj, Vector2(-15.0f, 7.0f));
            }
            
            // 명시적으로 Initialize 호출
            player->Initialize();
            
            mPlayers[charId] = player;
            
            std::string msg = "다른 플레이어 생성: ID=" + charId + " at (" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")\n";
            OutputDebugStringA(msg.c_str());
        }
    }

    void OtherPlayerManager::RemovePlayer(const std::string& charId)
    {
        auto it = mPlayers.find(charId);
        if (it != mPlayers.end())
        {
            if (it->second != nullptr)
            {
                delete it->second;
            }
            mPlayers.erase(it);
            
            std::string msg = "다른 플레이어 제거: " + charId + "\n";
            OutputDebugStringA(msg.c_str());
        }
    }

    void OtherPlayerManager::Clear()
    {
        for (auto& pair : mPlayers)
        {
            if (pair.second != nullptr)
            {
                delete pair.second;
            }
        }
        mPlayers.clear();
    }
}
