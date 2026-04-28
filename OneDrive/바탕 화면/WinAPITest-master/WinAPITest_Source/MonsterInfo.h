#pragma once

enum class MonsterState
{
	E_Idle,
	E_Patrol,
	E_Chase,
	E_Dead,
	E_NONE,
};

struct MonsterSpawnInfo
{
	int monsterId;
	int instanceId;
	
	float x;
	float y;
	int dir;
	int curHp;
	int maxHp;
	MonsterState state;
};


namespace Monster
{
	inline MonsterState SetState(int state)
	{
		switch (state)
		{
		case 1: return MonsterState::E_Idle;
		case 2: return MonsterState::E_Patrol;
		case 3: return MonsterState::E_Chase;
		case 4: return MonsterState::E_Dead;
		default: return MonsterState::E_NONE;
		}
	}
}