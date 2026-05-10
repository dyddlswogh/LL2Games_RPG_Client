#include "stbPlayer.h"
#include "InventoryManager.h"
#include "stbPlayerScript.h"


namespace stb
{
	Player::Player() : m_inven(InventoryManager::getInstance()), m_playerState(PlayerState::IDLE)
	{
		m_combatSystem.SetOwner(this);
	}
	void Player::Initialize()
	{
		GameObject::Initialize();
		m_quickSlotManager.Init();
	}

	void Player::Update()
	{
		GameObject::Update();
	}

	void Player::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void Player::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void Player::SetStat(BaseStat baseStat, DerivedStat derived, int cur_hp, int cur_mp, int remainAp)
	{
		m_stat.SetStat(baseStat, derived, cur_hp, cur_mp, remainAp);
	}

	void Player::SetPlayerInfo(PlayerIdentity playeridentity, PlayerProfile playerProfile, PlayerLocation playerlocation)
	{
		m_playerIdentity = playeridentity;
		m_playerProfile = playerProfile;
		m_playerLocation = playerlocation;

	}

	void Player::PlayAttackAnimation(int skillId)
	{


	}



}

