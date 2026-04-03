#include "stbInput.h"

namespace stb
{

	int ASCII[(UINT)eKeyCode::EnumsEnd] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_MBUTTON, VK_RBUTTON, VK_SPACE,
	};

	Input::Input()
		: Keys{}
	{

	}
	Input::~Input()
	{

	}
	void Input::Initialize()
	{
		CreateKeys();
	}

	void Input::Update()
	{
		for (auto& key : Keys)
		{
			UpdateKeys(key);
		}

	}

	void Input::UpdateKeys(Key& key)
	{
		if (GetFocus())
		{
			if (CheckIsKeyDown(key.keyCode))
			{
				IsKeyDown(key);
			}
			else
			{
				IsKeyUp(key);
			}
		}

	}

	void Input::CreateKeys()
	{
		for (int i = 0; i < (UINT)eKeyCode::EnumsEnd; i++)
		{
			Key key = {};
			key.bPressed = false;
			key.keyCode = (eKeyCode)i;
			key.State = eKeyState::None;

			Keys.push_back(key);
		}
	}

	void Input::IsKeyDown(Input::Key& key)
	{
		if (key.bPressed == true)
		{
			key.State = eKeyState::Pressed;
		}
		else
		{
			key.State = eKeyState::Down;
		}

		key.bPressed = true;
	}

	void Input::IsKeyUp(Input::Key& key)
	{
		if (key.bPressed == true)
		{
			key.State = eKeyState::Up;
		}
		else
		{
			key.State = eKeyState::None;
		}

		key.bPressed = false;
	}

	bool Input::CheckIsKeyDown(eKeyCode code)
	{
		return GetAsyncKeyState(ASCII[(UINT)code]) & 0x8000;
	}



}
