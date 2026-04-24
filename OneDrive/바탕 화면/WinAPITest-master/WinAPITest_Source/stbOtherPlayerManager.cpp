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

    bool OtherPlayerManager::HandleMovePacket(OtherPlayerMove& otherPlayerMove)
    {
        auto it = mPlayers.find(otherPlayerMove.playerId);
        if (it != mPlayers.end() && it->second != nullptr)
        {
            it->second->SetTargetPosition(otherPlayerMove.xPos, otherPlayerMove.yPos, otherPlayerMove.speed);
        }
        else
        {
            // 처음 보는 플레이어면 생성
            UpdatePlayer(otherPlayerMove.playerId, otherPlayerMove.xPos, otherPlayerMove.yPos);
        }

        return true;
        
    }

    bool OtherPlayerManager::UpdatePlayer(const std::string& charId, float x, float y)
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
                return false;
            }

            player->SetCharacterId(charId);
            
            // Transform 설정
            Transform* tr = player->GetComponent<Transform>();
            if (tr != nullptr)
            {
                tr->SetPosition(Vector2(x, y));
            }
            
            // DamonKnight 텍스처 사용
            // 지금은 DamonKnight로 통일하고 있지만 나중에는 받은 정보를 바탕으로 그려지도록 수정 필요
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

     
            // 명시적으로 Initialize 호출
            player->Initialize();
            
            mPlayers[charId] = player;
            
            std::string msg = "다른 플레이어 생성: ID=" + charId + " at (" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")\n";
            OutputDebugStringA(msg.c_str());
        }
        return true;
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
