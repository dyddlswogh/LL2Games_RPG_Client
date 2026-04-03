#pragma once
#include "..\\WinAPITest_Source\\stbScript.h"

namespace stb
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetFollowers(GameObject* head, GameObject* sword)
		{
			mHead = head;
			mSword = sword;
		}

	private:
		void Idle();
		void Move();
		void SyncFollowers(Vector2 pos);
	
	private:
		float mNetworkSendTimer;
		const float NETWORK_SEND_INTERVAL = 0.03f;
		GameObject* mHead;
		GameObject* mSword;
	};
}


