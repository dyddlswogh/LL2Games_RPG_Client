#pragma once
#include "CommonInclude.h"

namespace stb
{
	class Player;
}

class CombatSystem
{
public:
	CombatSystem();
	~CombatSystem() {};

	void SetPlayer(stb::Player* player) { m_player = player; }

	bool TryAttack(int skillId);
	bool CanAttack(int skillId);

private:
	stb::Player* m_player;



};

