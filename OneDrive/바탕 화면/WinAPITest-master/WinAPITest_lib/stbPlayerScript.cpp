#include "stbPlayerScript.h"
#include "stbInput.h"
#include "stbTransform.h"
#include "stbTime.h"
#include "stbGameObject.h"
#include "stbNetworkDebug.h"
#include "..\\WinAPITest_lib\\stbPlayer.h"
#include "PlayerManager.h"
#include "QuickSlotManager.h"
#include "UIManager.h"
#include "PlayerAnimationManager.h"
#include "stbSceneManager.h"
#include "stbPlayScene.h"


#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
#define M_TIME  stb::SingletonBase<stb::Time>::getInstance()
#define M_PLAYERMANAGER stb::SingletonBase<PlayerManager>::getInstance()
#define M_UIMANAGER stb::SingletonBase<UIManager>::getInstance()
#define M_PLAYERANIMMANAGER stb::SingletonBase<PlayerAnimationManager>::getInstance()
#define M_SCENEMANAGER stb::SingletonBase<stb::SceneManager>::getInstance()

namespace stb
{
	PlayerScript::PlayerScript()
		: mNetworkSendTimer(0.0f)
		, mHead(nullptr)
		, mSword(nullptr)
		, m_player(nullptr)
		, mAttackTimer(0.0f)
		, mAttackDuration(0.35f)
		, m_animator(nullptr)
		, m_quickSlotManager(nullptr)
	{

	}

	PlayerScript::~PlayerScript()
	{

	}

	void PlayerScript::Initialize()
	{	

	}	
		
	void PlayerScript::Update()
	{
		if (m_player == nullptr) return;

		if (m_player->GetState() == PlayerState::Attack)
		{
			Idle(false);          // 이동은 처리하되 상태는 바꾸지 않음
			UpdateAttackState();  // 공격 종료 시간 체크
			return;
		}

		Idle(true);
		HandleCombatInput();
		HandleInput();
		
	}	
		
	void PlayerScript::LateUpdate()
	{

	}	
		 
	void PlayerScript::Render(HDC hdc)
	{

	}

	void PlayerScript::SetAnimator()
	{
		if (m_player == nullptr) return;

		m_animator = m_player->GetAnimator();
	}

	void PlayerScript::UpdateAttackState()
	{
		stb::Player* player = M_PLAYERMANAGER->GetLocalPlayer();

		if (player == nullptr)
			return;

		mAttackTimer += M_TIME->GetDeltaTime();

		if (mAttackTimer < mAttackDuration)
			return;

		mAttackTimer = 0.0f;

		if (IsMoveInputPressed())
		{
			player->SetState(PlayerState::Walk);
			OutputDebugStringA("Attack End -> Move\n");
		}
		else
		{
			player->SetState(PlayerState::Idle);
			OutputDebugStringA("Attack End -> Idle\n");
		}
	}

	void PlayerScript::Idle(bool changeState)
	{
		if (M_UIMANAGER->IsInputFocused())
			return; //채팅 입력중 -> 이동/공격 차단
		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (tr == nullptr)
			return;

		Vector2 pos = tr->GetPosition();
		bool moved = false;

		if (m_player->GetState() != PlayerState::Attack)
		{
			if (M_INPUT->GetAction(eActionCode::MoveRight))
			{
				pos.x += 100.0f * M_TIME->GetDeltaTime();
				m_player->SetFacing(FacingDirection::Right);
				m_animator->SetFlipX(true);
				moved = true;
			}

			if (M_INPUT->GetAction(eActionCode::MoveLeft))
			{
				pos.x -= 100.0f * M_TIME->GetDeltaTime();
				m_player->SetFacing(FacingDirection::Left);
				m_animator->SetFlipX(false);
				moved = true;
			}

			if (M_INPUT->GetAction(eActionCode::MoveUp))
			{
				pos.y -= 100.0f * M_TIME->GetDeltaTime();
				moved = true;
			}

			if (M_INPUT->GetAction(eActionCode::MoveDown))
			{
				pos.y += 100.0f * M_TIME->GetDeltaTime();
				moved = true;
			}

			tr->SetPosition(pos);
		}
		
		if (changeState)
		{
			stb::Player* player = M_PLAYERMANAGER->GetLocalPlayer();
			if (player != nullptr)
			{
				if (moved)
					player->SetState(PlayerState::Walk);
				else
					player->SetState(PlayerState::Idle);
			}
		}

		SyncFollowers(pos);

		if (moved)
		{
			mNetworkSendTimer += M_TIME->GetDeltaTime();

			if (mNetworkSendTimer >= NETWORK_SEND_INTERVAL)
			{
				auto netMgr = stb::NetworkManager::getInstance();
				if (netMgr != nullptr && netMgr->IsConnected())
				{
					stb::SendPlayerMove(pos.x, pos.y, 100.0f);
				}

				mNetworkSendTimer = 0.0f;
			}
		}
		else
		{
			mNetworkSendTimer = 0.0f;
		}
	}

	void PlayerScript::Move()
	{

	}

	void PlayerScript::Attack()
	{
		stb::Player* player = m_player;

		if (player == nullptr)
			return;

		if (player->GetCombatSystem() == nullptr)
			return;

		if (player->GetState() == PlayerState::Attack)
			return;

		if (player->GetCombatSystem()->TryBasicAttack())
		{
			player->SetState(PlayerState::Attack);
			mAttackTimer = 0.0f;

			OutputDebugStringA("Player Attack Start\n");
		}

	}

	void PlayerScript::Jump() 
	{
		if (m_player == nullptr)
			return;

		if (m_player->GetCombatSystem() == nullptr)
			return;

		if (m_player->GetState() == PlayerState::Jump)
			return;

		m_player->SetState(PlayerState::Jump);
		
	}

	void PlayerScript::PickUp()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (tr == nullptr)
			return;

		stb::Scene* scene = M_SCENEMANAGER->GetActiveScene();

		if (scene == nullptr)
			return;

		PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
		if (playScene == nullptr)
			return;

		DropItemManager* dropManager = playScene->GetDropItemManager();
		if (dropManager == nullptr)
			return;

		dropManager->RequestPickup(tr->GetPosition());
	}

	void PlayerScript::HandleInput()
	{
		KeyBindInfo bindInfo;

		if (M_INPUT->GetPressedBind(bindInfo))
		{
			ExecuteBind(bindInfo);
		}
	}

	void PlayerScript::HandleCombatInput()
	{
		if (M_INPUT->GetActionDown(eActionCode::Attack))
		{
			DebugMsg = "HandleComabatInput is Pressed\n";
			OutputDebugStringA(DebugMsg.c_str());	
			Attack();
		}
	}

	void PlayerScript::ExecuteBind(const KeyBindInfo& bindInfo)
	{
		switch (bindInfo.type)
		{
		case eBindType::Action:
			ExecuteAction((eActionCode)bindInfo.value);
			break;
		case eBindType::Skill:
			// TODO : 스킬 사용 요청
			// SkillManager::GetInstance()->UseSkill(bindInfo.value);
			OutputDebugStringA("Skill Execute\n");
			break;
		case eBindType::Item:
			// TODO : 아이템 사용 요청
			// ItemManager::GetInstance()->UseItem(bindInfo.value);
			OutputDebugStringA("Item Execute\n");
			break;
		case eBindType::UI:
			OutputDebugStringA("UI Execute\n");
			break;
		case eBindType::QuickSlot:
			OutputDebugStringA("QuickSlot Execute\n");
			m_player->GetQuickSlotManager()->UseSlot(bindInfo.value);
			break;
		default:
			break;
		}
	}

	void PlayerScript::ExecuteAction(eActionCode action)
	{
		switch (action)
		{
		case eActionCode::Interact:
			OutputDebugStringA("Action : Interact\n");
			// TODO : 상호작용 요청
			break;
		case eActionCode::Jump:
			Jump();
			OutputDebugStringA("Action : Jump\n");
			// TODO : 점프 처리
			break;
		case eActionCode::Inventory:
			OutputDebugStringA("Action : Inventory\n");
			UIManager::getInstance()->ToggleInventory();
			// TODO : 인벤토리 UI 열기
			break;
		case eActionCode::SkillWindow:
			OutputDebugStringA("Action : SkillWindow\n");
			// TODO : 스킬창 UI 열기
			break;
		case eActionCode::PickUp:
			OutputDebugStringA("Action : PickUp\n");
			PickUp();
			break;

		default:
			break;
		}
	}

	

	void PlayerScript::SyncFollowers(Vector2 pos)
	{
		if (mHead)
		{
			Transform* tr = mHead->GetComponent<Transform>();
			if (tr) tr->SetPosition(pos);
		}
		if (mSword)
		{
			Transform* tr = mSword->GetComponent<Transform>();
			if (tr) tr->SetPosition(Vector2(pos.x - 15.0f, pos.y + 7.0f));
		}
	}

	bool PlayerScript::IsMoveInputPressed() const
	{
		return M_INPUT->GetAction(eActionCode::MoveRight) ||
			M_INPUT->GetAction(eActionCode::MoveLeft) ||
			M_INPUT->GetAction(eActionCode::MoveUp) ||
			M_INPUT->GetAction(eActionCode::MoveDown);
	}

}
