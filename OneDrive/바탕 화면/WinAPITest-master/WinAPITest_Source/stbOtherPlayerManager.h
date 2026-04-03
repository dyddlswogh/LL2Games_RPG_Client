#pragma once
#include "stbSingletonBase.h"
#include "stbOtherPlayer.h"
#include <map>
#include <string>

namespace stb
{
    // 다른 플레이어들을 관리하는 매니저
    class OtherPlayerManager : public SingletonBase<OtherPlayerManager>
    {
    public:
        OtherPlayerManager();
        ~OtherPlayerManager();

        // 다른 플레이어 추가 또는 업데이트
        void UpdatePlayer(const std::string& charId, float x, float y);

        // 다른 플레이어 제거
        void RemovePlayer(const std::string& charId);

        // 모든 플레이어 제거
        void Clear();

        // 플레이어 맵 접근
        std::map<std::string, OtherPlayer*>& GetPlayers() { return mPlayers; }

    private:
        std::map<std::string, OtherPlayer*> mPlayers;
    };
}
