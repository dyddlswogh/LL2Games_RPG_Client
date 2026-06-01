#include "stbPlayer.h"
#include "InventoryManager.h"
#include "stbPlayerScript.h"
#include "..\\WinAPITest_Source\\\PlayerAnimationManager.h"



#define M_PLAYERANIMMANAGER stb::SingletonBase<PlayerAnimationManager>::getInstance()

namespace stb
{
	Player::Player() : m_inven(InventoryManager::getInstance()), m_playerState(PlayerState::None),
					   m_transform(nullptr), m_animator(nullptr), m_damageText(nullptr), m_collider(nullptr), m_script(nullptr), m_isLocalPlayer(false)
	{
		m_combatSystem.SetOwner(this);
	}
	void Player::Initialize()
	{
		GameObject::Initialize();
		m_transform = AddComponent<stb::Transform>();
		m_transform->SetPosition(Vector2(300.0f, 100.0f));
		m_animator = AddComponent<stb::Animator>();
		m_damageText = AddComponent<stb::DamageText>();
		m_collider = AddComponent<stb::BoxCollider2D>();
		m_script = AddComponent<PlayerScript>();
		m_quickSlotManager.Init();

		//m_quickSlotManager.SetOwner(this);
		m_script->SetOwner(this);
		m_script->SetPlayer(this);
		m_script->SetAnimator();
		
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

	void Player::Render(stbD2DRenderer& renderer)
	{
		GameObject::Render(renderer);
	}

	void Player::SetStat(BaseStat baseStat, DerivedStat derived, ExpStat expStat, int cur_hp, int cur_mp, int remainAp)
	{
		m_stat.SetStat(baseStat, derived, expStat, cur_hp, cur_mp, remainAp);
	}

	void Player::SetPlayerInfo(PlayerIdentity playeridentity, PlayerProfile playerProfile, PlayerLocation playerlocation)
	{
		m_playerIdentity = playeridentity;
		m_playerProfile = playerProfile;
		m_playerLocation = playerlocation;

		JobType jobType = static_cast<JobType>(m_playerProfile.job);
		WeaponType weaponType = WeaponType::One_Hand;

		M_PLAYERANIMMANAGER->Init();

		M_PLAYERANIMMANAGER->SetupPlayerAnimations(this, jobType, weaponType);

		SetState(PlayerState::Idle);
	}

	void Player::PlayAttackAnimation(int skillId)
	{


	}

	void Player::SetState(PlayerState state)
	{
		if (m_playerState == state)
		{

			return;
		}
			

		m_playerState = state;
		
		switch (m_playerState)
		{
		case PlayerState::Idle:
			m_animator->PlayAnimation(L"alert", true);
			break;

		case PlayerState::Alert:
			m_animator->PlayAnimation(L"alert", true);
			break;

		case PlayerState::Walk:
			m_animator->PlayAnimation(L"walk", true);
			break;

		case PlayerState::Jump:
			m_animator->PlayAnimation(L"jump", false);
			break;

		case PlayerState::Prone:
			m_animator->PlayAnimation(L"prone", true);
			break;

		case PlayerState::Rope:
			m_animator->PlayAnimation(L"rope", true);
			break;

		case PlayerState::Ladder:
			m_animator->PlayAnimation(L"ladder", true);
			break;

		case PlayerState::Attack:
			m_animator->PlayAnimation(L"swingT1", true);
			//PlayAttackAnimation(static_cast<int>(m_weaponType));
			break;
		}
	}

	void Player::OnDamaged(int damage, int curHp)
	{
		m_stat.SetCurHp(curHp);
		ShowDamageText(damage);

		// 필요하면 여기서 피격 상태/무적/넉백도 나중에 추가
	}

	void Player::ShowDamageText(int damage)
	{
		if (m_damageText != nullptr)
			m_damageText->AddDamage(damage);
	}

}

