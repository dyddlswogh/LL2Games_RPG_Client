#pragma once
#include "CommonInclude.h"
#include "MonsterInfo.h"
#include "stbMath.h"

class stbD2DRenderer;

class Monster
{
public:
    void Init(int instanceId, int templateId, const stb::math::Vector2 & pos, int hp, int maxHp);
    void Update(float deltaTime);
    void Render(stbD2DRenderer& renderer);

    void SetState(MonsterState state);
    void SetPosition(float x, float y);
    void OnDamaged(int damage, int curHp, bool dead);
    void OnMove(float x, float y, int dir);

    int GetInstanceId() const { return m_instanceId; }

private:
    int m_instanceId = 0;   // 서버 개체 ID
    int m_templateId = 0;   // 몬스터 종류 ID

    stb::math::Vector2 m_pos{};
    stb::math::Vector2 m_targetPos{};     // 서버 이동 패킷 받은 위치
    int m_dir = 1;

    int m_curHp = 0;
    int m_maxHp = 0;

    MonsterState m_state = MonsterState::E_Idle;

    //MonsterResource* m_resource = nullptr;
   // AnimationPlayer m_animator;
};

