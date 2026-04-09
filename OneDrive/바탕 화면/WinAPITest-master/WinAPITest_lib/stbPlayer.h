#pragma once
#include "..\\WinAPITest_Source\\stbGameObject.h"
#include "..\\WinAPITest_Source\\Stat.h"
#include "..\\WinAPITest_Source\\\InventoryManager.h"




namespace stb
{
	class Player : public GameObject
	{
	public:
		Player();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetStat(BaseStat baseStat, DerivedStat derived, int cur_hp, int cur_mp, int remainAp);
		void SetPlayerInfo(PlayerIdentity playeridentity, PlayerProfile playerProfile, PlayerLocation playerlocation);
	public:
		Stat* GetStat() { return &m_stat; }
		InventoryManager* GetInvenManager() { return m_inven; }

		const PlayerIdentity* GetPlayerIdentity() const { return &m_playerIdentity; }
		PlayerIdentity* GetPlayerIdentity() { return &m_playerIdentity; }
		
		const PlayerProfile* GetPlayerProfile() const { return &m_playerProfile; }
		PlayerProfile* GetPlayerProfile() { return &m_playerProfile; }

		PlayerLocation* GetPlayerLocation() { return &m_playerLocation; }


	private:

		// 
		Stat m_stat;
		InventoryManager* m_inven;

		// 플레이어 계정 정보
		PlayerIdentity m_playerIdentity;
		// 플레이어 프로필 정보
		PlayerProfile m_playerProfile;
		// 플레이어 위치 정보
		PlayerLocation m_playerLocation;

		PlayerState m_playerState;


	};

}

