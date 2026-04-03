#include "stbPlayerScript.h"
#include "stbInput.h"
#include "stbTransform.h"
#include "stbTime.h"
#include "stbGameObject.h"
#include "stbNetworkDebug.h"

#define M_Input stb::SingletonBase<stb::Input>::getInstance()
#define M_Time  stb::SingletonBase<stb::Time>::getInstance()

namespace stb
{
	PlayerScript::PlayerScript()
		: mNetworkSendTimer(0.0f)
		, mHead(nullptr)
		, mSword(nullptr)
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
		Idle();
	}	
		
	void PlayerScript::LateUpdate()
	{

	}	
		
	void PlayerScript::Render(HDC hdc)
	{

	}

	void PlayerScript::Idle()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		if (tr == nullptr)
		{
			return;
		}

		Vector2 pos = tr->GetPosition();
		bool moved = false;

		if (M_Input->GetKey(eKeyCode::Right))
		{
			pos.x += 100.0f * M_Time->GetDeltaTime();
			moved = true;
		}

		if (M_Input->GetKey(eKeyCode::Left))
		{
			pos.x -= 100.0f * M_Time->GetDeltaTime();
			moved = true;
		}

		if (M_Input->GetKey(eKeyCode::Up))
		{
			pos.y -= 100.0f * M_Time->GetDeltaTime();
			moved = true;
		}

		if (M_Input->GetKey(eKeyCode::Down))
		{
			pos.y += 100.0f * M_Time->GetDeltaTime();
			moved = true;
		}

		tr->SetPosition(pos);

		SyncFollowers(pos);

		// 이동했으면 서버에 패킷 전송 (throttling 적용)
		if (moved)
		{
			mNetworkSendTimer += M_Time->GetDeltaTime();
			
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
			mNetworkSendTimer = 0.0f;  // 멈추면 타이머 리셋
		}
	}

	void PlayerScript::Move()
	{

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

}
