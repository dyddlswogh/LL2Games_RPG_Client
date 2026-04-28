#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"
#include "Monster.h"

class StbD2DRenderer;

class MonsterManager : public stb::SingletonBase<MonsterManager>
{
public:
    void SpawnMonster(const MonsterSpawnInfo& info);
    void RemoveMonster(int instanceId);
    void MoveMonster(int instanceId, float x, float y, int dir);
    //void ApplyMonsterDamage(const MonsterHitInfo& info);

    void Update(float deltaTime);
    void Render(StbD2DRenderer& renderer);

private:
    std::unordered_map<int, std::unique_ptr<Monster>> m_monsters;
};

