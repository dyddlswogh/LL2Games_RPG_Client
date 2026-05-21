#include "stbInput.h"
#include "UIManager.h"
#include "PlayerManager.h"

#define M_PLAYERMANAGER stb::SingletonBase<PlayerManager>::getInstance()
namespace stb
{
	int ASCII[(UINT)eKeyCode::EnumsEnd] =
	{
		'F1','F2','F3','F4','F5','F6','F7','F8','F9','F10','F11','F12',
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_LBUTTON, VK_MBUTTON, VK_RBUTTON, VK_SPACE, VK_LMENU, VK_LCONTROL,
		VK_RETURN, VK_RETURN,
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
		CreateDefaultBindings();
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
		else
		{
			key.State = eKeyState::None;
			key.bPressed = false;
		}
	}

	void Input::CreateKeys()
	{
		Keys.clear();
		Keys.reserve((UINT)eKeyCode::EnumsEnd);

		for (int i = 0; i < (UINT)eKeyCode::EnumsEnd; i++)
		{
			Key key = {};
			key.bPressed = false;
			key.keyCode = (eKeyCode)i;
			key.State = eKeyState::None;

			Keys.push_back(key);
		}
	}

	void Input::CreateDefaultBindings()
	{
		m_keyBindings.clear();

		// 이동 액션 바인딩
		m_keyBindings[eKeyCode::Left] = { eBindType::Action, (int)eActionCode::MoveLeft };
		m_keyBindings[eKeyCode::Right] = { eBindType::Action, (int)eActionCode::MoveRight };
		m_keyBindings[eKeyCode::Up] = { eBindType::Action, (int)eActionCode::MoveUp };
		m_keyBindings[eKeyCode::Down] = { eBindType::Action, (int)eActionCode::MoveDown };


		// 기본 액션 바인딩
		m_keyBindings[eKeyCode::LALT] = { eBindType::Action, (int)eActionCode::Jump };
		m_keyBindings[eKeyCode::LCONTROL] = { eBindType::Action, (int)eActionCode::Attack };
		m_keyBindings[eKeyCode::SPACE] = { eBindType::Action, (int)eActionCode::Interact };
		m_keyBindings[eKeyCode::I] = { eBindType::Action, (int)eActionCode::Inventory };
		m_keyBindings[eKeyCode::K] = { eBindType::Action, (int)eActionCode::SkillWindow };
		m_keyBindings[eKeyCode::T] = { eBindType::Action, (int)eActionCode::Trade };

		//교환 취소 임시(X)
		m_keyBindings[eKeyCode::X] = { eBindType::Action, (int)eActionCode::TradeCancel };


		// 예시
		// m_keyBindings[eKeyCode::Q] = { eBindType::Skill, 1001001 };
		// m_keyBindings[eKeyCode::R] = { eBindType::Item, 2000000 };
	}

	bool Input::GetPressedBind(KeyBindInfo& outBindInfo)
	{
		for (int i = 0; i < (UINT)eKeyCode::EnumsEnd; i++)
		{
			eKeyCode code = (eKeyCode)i;

			if (GetKeyDown(code) == false)
				continue;

			auto it = m_keyBindings.find(code);
			if (it == m_keyBindings.end())
				continue;

			outBindInfo = it->second;
			return true;
		}

		return false;
	}


	void Input::BindKey(eKeyCode code, const KeyBindInfo& bindInfo)
	{
		m_keyBindings[code] = bindInfo;
	}

	void Input::UnbindKey(eKeyCode code)
	{
		auto it = m_keyBindings.find(code);
		if (it != m_keyBindings.end())
		{
			m_keyBindings.erase(it);
		}
	}

	bool Input::TryGetBindInfo(eKeyCode code, KeyBindInfo& outBindInfo) const
	{
		auto it = m_keyBindings.find(code);
		if (it == m_keyBindings.end())
		{
			return false;
		}

		outBindInfo = it->second;
		return true;
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

	bool Input::GetAction(eActionCode action)
	{
		for (const auto& pair : m_keyBindings)
		{
			if (pair.second.type != eBindType::Action)
				continue;

			if (pair.second.value != (int)action)
				continue;

			if (GetKey(pair.first))
				return true;
		}

		return false;
	}

	bool Input::GetActionDown(eActionCode action)
	{
		for (const auto& pair : m_keyBindings)
		{
			if (pair.second.type != eBindType::Action)
				continue;

			if (pair.second.value != (int)action)
				continue;

			if (GetKeyDown(pair.first))
				return true;
		}

		return false;
	}

	bool Input::GetActionUp(eActionCode action)
	{
		for (const auto& pair : m_keyBindings)
		{
			if (pair.second.type != eBindType::Action)
				continue;

			if (pair.second.value != (int)action)
				continue;

			if (GetKeyUp(pair.first))
				return true;
		}

		return false;
	}
}