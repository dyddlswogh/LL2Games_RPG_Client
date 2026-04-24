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

private:
	stb::Player* m_local_player;
	std::unordered_map<int, stb::Player*> m_otherPlayers;



};

