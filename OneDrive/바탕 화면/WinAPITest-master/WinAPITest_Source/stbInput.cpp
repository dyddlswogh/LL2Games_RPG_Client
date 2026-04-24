#include "stbInput.h"
#include "UIManager.h"
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
		CreateDefaultBindings();
	}

	void Input::Update()
	{
		for (auto& key : Keys)
		{
			UpdateKeys(key);
		}

		ProcessKeyBindings();
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

		// 이동 액샌 바인딩
		m_keyBindings[eKeyCode::Left] = { eBindType::Action, (int)eActionCode::MoveLeft };
		m_keyBindings[eKeyCode::Right] = { eBindType::Action, (int)eActionCode::MoveRight };
		m_keyBindings[eKeyCode::Up] = { eBindType::Action, (int)eActionCode::MoveUp };
		m_keyBindings[eKeyCode::Down] = { eBindType::Action, (int)eActionCode::MoveDown };


		// 기본 액션 바인딩
		m_keyBindings[eKeyCode::Space] = { eBindType::Action, (int)eActionCode::Jump };
		m_keyBindings[eKeyCode::A] = { eBindType::Action, (int)eActionCode::Attack };
		m_keyBindings[eKeyCode::E] = { eBindType::Action, (int)eActionCode::Interact };
		m_keyBindings[eKeyCode::I] = { eBindType::Action, (int)eActionCode::Inventory };
		m_keyBindings[eKeyCode::K] = { eBindType::Action, (int)eActionCode::SkillWindow };

		// 예시
		// m_keyBindings[eKeyCode::Q] = { eBindType::Skill, 1001001 };
		// m_keyBindings[eKeyCode::R] = { eBindType::Item, 2000000 };
	}

	void Input::ProcessKeyBindings()
	{
		for (int i = 0; i < (UINT)eKeyCode::EnumsEnd; i++)
		{
			eKeyCode code = (eKeyCode)i;

			if (GetKeyDown(code) == false)
				continue;

			auto it = m_keyBindings.find(code);
			if (it == m_keyBindings.end())
				continue;

			ExecuteBind(it->second);
		}
	}

	void Input::ExecuteBind(const KeyBindInfo& bindInfo)
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

	void Input::ExecuteAction(eActionCode action)
	{
		switch (action)
		{
		case eActionCode::Interact:
			OutputDebugStringA("Action : Interact\n");
			// TODO : 상호작용 요청
			break;
		case eActionCode::Attack:
			OutputDebugStringA("Action : Attack\n");
			// TODO : 점프 처리
			break;
		case eActionCode::Jump:
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