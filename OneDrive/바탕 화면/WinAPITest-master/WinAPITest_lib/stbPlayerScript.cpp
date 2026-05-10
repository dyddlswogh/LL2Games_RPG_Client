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


#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
#define M_TIME  stb::SingletonBase<stb::Time>::getInstance()
#define M_PLAYERMANAGER stb::SingletonBase<PlayerManager>::getInstance()
#define M_UIMANAGER stb::SingletonBase<UIManager>::getInstance()


namespace stb
{
	PlayerScript::PlayerScript()
		: mNetworkSendTimer(0.0f)
		, mHead(nullptr)
		, mSword(nullptr)
		, m_player(nullptr)
		, mAttackTimer(0.0f)
		, mAttackDuration(0.35f)
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
		stb::Player* player = M_PLAYERMANAGER->GetLocalPlayer();

		if (player != nullptr && player->GetState() == PlayerState::ATTACK)
		{
			Idle(false);          // 이동은 처리하되 상태는 바꾸지 않음
			UpdateAttackState();  // 공격 종료 시간 체크
			return;
		}

		Idle(true);
		HandleInput();
		HandleCombatInput();
	}	
		
	void PlayerScript::LateUpdate()
	{

	}	
		 
	void PlayerScript::Render(HDC hdc)
	{

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
			player->SetState(PlayerState::MOVE);
			OutputDebugStringA("Attack End -> Move\n");
		}
		else
		{
			player->SetState(PlayerState::IDLE);
			OutputDebugStringA("Attack End -> Idle\n");
		}
	}

	void PlayerScript::Idle(bool changeState)
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (tr == nullptr)
			return;

		Vector2 pos = tr->GetPosition();
		bool moved = false;

		if (M_INPUT->GetAction(eActionCode::MoveRight))
		{
			pos.x += 100.0f * M_TIME->GetDeltaTime();
			moved = true;
		}

		if (M_INPUT->GetAction(eActionCode::MoveLeft))
		{
			pos.x -= 100.0f * M_TIME->GetDeltaTime();
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

		if (changeState)
		{
			stb::Player* player = M_PLAYERMANAGER->GetLocalPlayer();
			if (player != nullptr)
			{
				if (moved)
					player->SetState(PlayerState::MOVE);
				else
					player->SetState(PlayerState::IDLE);
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
		stb::Player* player = M_PLAYERMANAGER->GetLocalPlayer();

		if (player == nullptr)
			return;

		if (player->GetCombatSystem() == nullptr)
			return;

		if (player->GetState() == PlayerState::ATTACK)
			return;

		if (player->GetCombatSystem()->TryBasicAttack())
		{
			player->SetState(PlayerState::ATTACK);
			mAttackTimer = 0.0f;

			OutputDebugStringA("Player Attack Start\n");
		}

	}

	void PlayerScript::Jump() 
	{

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
