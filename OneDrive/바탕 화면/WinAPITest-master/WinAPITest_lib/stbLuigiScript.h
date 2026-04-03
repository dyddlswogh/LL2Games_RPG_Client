#pragma once
#include "stbScript.h"

namespace stb
{
	class LuigiScript : public Script
	{
	public:
		enum class Direction
		{
			Up,
			Down,
			Left,
			Right,
			EnumEnd
		};



		LuigiScript();
		~LuigiScript();
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void Idle();
		void Move(Direction dir, class Transform* tr);

	private:
		Direction mDirection;
		class Animator* mAnimator;

		float mTime;
	
	};
}

