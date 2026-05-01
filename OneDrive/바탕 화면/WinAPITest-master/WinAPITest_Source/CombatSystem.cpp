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
 
    stb::FacingDirection direction = m_player->GetFacing();

    if (direction == stb::FacingDirection::Left)
        dir = -1;
    else
        dir = 1;

    // 서버에 공격 패킷 보내기
    CombatPacketHandler::SendUseSkill(skillId, dir);
    return true;
}

bool CombatSystem::TryBasicAttack()
{
    if (m_player == nullptr)
        return false;

    if (!CanBasicAttack())
        return false;

    AttackDirection dir = AttackDirection::Right;

    if (m_player->GetFacing() == stb::FacingDirection::Left)
        dir = AttackDirection::Left;

    // 서버에 공격 패킷 보내기
    CombatPacketHandler::SendBasicAttack(static_cast<int>(dir));
    m_player->SetState(PlayerState::ATTACK);
    m_player->PlayAttackAnimation(m_player->GetWeaponTypeToInt());
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

    // 스킬 쿨타임 여부 확인 추가 필요
    return true;
}

bool CombatSystem::CanBasicAttack()
{
    if (m_player == nullptr)
        return false;

    if (m_player->IsDead())
        return false;

    if (m_player->IsAttacking())
        return false;

    return true;
}


