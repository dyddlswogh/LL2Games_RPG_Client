#include "CombatSystem.h"
#include "..\\WinAPITest_lib\\stbPlayer.h"
#include "CombatPacketHandler.h"
#include "stbMath.h"



CombatSystem::CombatSystem() : m_player(nullptr)
{

}

bool CombatSystem::TryAttack(int skillId)
{
    if (!CanAttack(skillId))
        return false;

  
    // 플레이어 공격 방향 나중에 추가
    int dir = 1;
    //int dir = m_owner->Get

    // 서버에 공격 패킷 보내기
    CombatPacketHandler::SendUseSkill(skillId,dir);
    return true;
}

bool CombatSystem::CanAttack(int skillId)
{
    if (m_player == nullptr)
        return false;

    if (m_player->IsDead())
        return false;

    if (m_player->IsAttacking())
        return false;

    return true;
}


