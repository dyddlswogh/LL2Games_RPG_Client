#pragma once
#include "..\\WinAPITest_Source\\stbGameObject.h"
#include "..\\WinAPITest_Source\\Stat.h"
#include "..\\WinAPITest_Source\\\InventoryManager.h"
#include "..\\WinAPITest_Source\\\CombatSystem.h"
#include "..\\WinAPITest_Source\\\QuickSlotManager.h"


namespace stb
{

	enum class FacingDirection
	{
		Left = -1,
		Right = 1
	};

	// 이거 나중에 Weapon_Info로 옮겨야함 
	enum class WeaponType
	{
		None	 = 290000,
		One_Hand = 290001,
		Two_Hand = 290002,
		Bow		 = 290003,	
	};

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
		void PlayAttackAnimation(int skillId);
	public:
		Stat* GetStat() { return &m_stat; }
		InventoryManager* GetInvenManager() { return m_inven; }

		const PlayerIdentity* GetPlayerIdentity() const { return &m_playerIdentity; }
		PlayerIdentity* GetPlayerIdentity() { return &m_playerIdentity; }
		
		const PlayerProfile* GetPlayerProfile() const { return &m_playerProfile; }
		PlayerProfile* GetPlayerProfile() { return &m_playerProfile; }

		PlayerLocation* GetPlayerLocation() { return &m_playerLocation; }

		CombatSystem* GetCombatSystem() { return &m_combatSystem; }

		// 플레이어의 상태가 Dead인지 확인하는 함수
		bool IsDead() { return m_playerState == PlayerState::DEAD; };

		// 플레이어의 상태가 Attack인지 확인하는 함수
		bool IsAttacking() { return m_playerState == PlayerState::ATTACK; }
		void SetState(PlayerState state) { m_playerState = state; }

		QuickSlotManager* GetQuickSlotManager() { return &m_quickSlotManager; }

		void SetFacing(FacingDirection facing) { m_facing = facing; }
		FacingDirection GetFacing() { return m_facing; }
		
		int GetWeaponTypeToInt() { return static_cast<int>(m_weaponType); }
		WeaponType GetWeaponType() { return m_weaponType; }
	private:
		WeaponType m_weaponType = WeaponType::None;
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

		// 플레이어 전투 시스템
		CombatSystem  m_combatSystem;

		QuickSlotManager m_quickSlotManager;
		
		FacingDirection m_facing = FacingDirection::Right;
	};

}

