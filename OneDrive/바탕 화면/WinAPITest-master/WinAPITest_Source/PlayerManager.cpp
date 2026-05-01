#include "PlayerManager.h"



void PlayerManager::PlayAttackAnimation(int playerId, int skillId)
{
	// 로컬 플레이어면 플레이어의 상태를 공격중으로 변경하고 애니메이션을 실행

	// 로컬플레이어의 경우 넘긴다.
    if (m_local_player == nullptr)
        return;

    if (m_local_player->GetPlayerIdentity()->charId == playerId)
        return;

    stb::Player* player = FindPlayer(playerId);
    if (player == nullptr)
        return;

    player->SetState(PlayerState::ATTACK);
    player->PlayAttackAnimation(skillId);
}

stb::Player* PlayerManager::FindPlayer(int playerId)
{
    auto player = m_otherPlayers.find(playerId);

    if (player == m_otherPlayers.end())
    {
        return nullptr;
    }

    return player->second;
}
