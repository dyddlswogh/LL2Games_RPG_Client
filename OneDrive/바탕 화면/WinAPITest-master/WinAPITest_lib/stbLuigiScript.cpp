#include "stbLuigiScript.h"
#include "stbTransform.h"
#include "stbTime.h"
#include "stbTexture.h"
#include "stbAnimator.h"
#include "stbGameObject.h"


#define M_TIME stb::SingletonBase<stb::Time>::getInstance()

namespace stb
{
	LuigiScript::LuigiScript()
		: mDirection(Direction::EnumEnd)
		, mTime(0.0f)
		, mAnimator(nullptr)
	{

	}

	LuigiScript::~LuigiScript()
	{

	}

	void LuigiScript::Initialize()
	{

	}

	void LuigiScript::Update()
	{
		
		Idle();
	}

	void LuigiScript::LateUpdate()
	{

	}

	void LuigiScript::Render(HDC hdc)
	{

	}

	void LuigiScript::Idle()
	{
		mTime += M_TIME->GetDeltaTime();

		if (mTime > 2.0f)
		{
			int ranInt = rand() % 5 + 1;
	
			switch (ranInt)
			{
			case 1:
				mDirection = Direction::Up;
				break;
			case 2:
				mDirection = Direction::Down;
				break;
			case 3:
				mDirection = Direction::Left;
				break;
			case 4:
				mDirection = Direction::Right;
				break;
			}
			mTime = 0.0f;
		}
		
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Move(mDirection, tr);
	}
	void LuigiScript::Move(Direction dir, Transform* tr)
	{
		Vector2 pos = tr->GetPosition();
		switch (dir)
		{
		case Direction::Up:
			pos.y -= 100.0f * M_TIME->GetDeltaTime();
			break;
		case Direction::Down:
			pos.y += 100.0f * M_TIME->GetDeltaTime();
			break;
		case Direction::Left:
			pos.x -= 100.0f * M_TIME->GetDeltaTime();
			break;
		case Direction::Right:
			pos.x += 100.0f * M_TIME->GetDeltaTime();
			break;
		}

		tr->SetPosition(pos);
	}
}