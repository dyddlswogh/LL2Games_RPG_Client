#pragma once
#include "CommonInclude.h"
#include "..\\WinAPITest_lib\\\stbPlayer.h"
#include "stbSingletonBase.h"

class PlayerManager : public stb::SingletonBase<PlayerManager>
{
public:


public:
	stb::Player* GetLocalPlayer() { return m_local_player; }

	void SetLocalPlayer(stb::Player* player) { m_local_player = player; }
	void PlayAttackAnimation(int playerId, int skillId);

	stb::Player* FindPlayer(int playerId);
private:
	stb::Player* m_local_player;
	std::unordered_map<int, stb::Player*> m_otherPlayers;



};

