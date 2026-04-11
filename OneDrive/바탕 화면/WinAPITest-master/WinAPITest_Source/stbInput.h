#pragma once
#include "stbSingletonBase.h"
#include "CommonInclude.h"

namespace stb
{
	enum class eKeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		Left, Right, Up, Down,
		LButton, MButton, RButton, Space,
		EnumsEnd,
	};

	enum class eKeyState
	{
		Down,
		Pressed,
		Up,
		None,

	};

	enum class eActionCode
	{
		None,
		Interact,
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Jump,
		Attack,
		PickUp,
		Inventory,
		CharacterInfo,
		SkillWindow,
		QuestWindow,
		WorldMap,
		EnumsEnd,
	};

	enum class eBindType
	{
		None,
		Action,   // Interact °°Àº °Í
		Skill,
		Item,
		UI,
	};

	struct KeyBindInfo
	{
		eBindType type;
		int value;
	};

	struct EnumClassHash
	{
		template <typename T>
		std::size_t operator()(T t) const
		{
			return static_cast<std::size_t>(t);
		}
	};

	class Input : public SingletonBase<Input>
	{

	public:
		struct Key
		{
			eKeyCode keyCode;
			eKeyState State;
			bool bPressed;
		};

		Input();
		~Input();

		__forceinline bool GetKeyDown(eKeyCode eCode) { return Keys[(UINT)eCode].State == eKeyState::Down; }
		__forceinline bool GetKeyUp(eKeyCode eCode) { return Keys[(UINT)eCode].State == eKeyState::Up; }
		__forceinline bool GetKey(eKeyCode eCode) { return Keys[(UINT)eCode].State == eKeyState::Pressed; }


		void Initialize();
		void Update();
		void UpdateKeys(Key& key);
		void CreateKeys();

		void IsKeyDown(Input::Key& key);
		void IsKeyUp(Input::Key& key);

		bool CheckIsKeyDown(eKeyCode code);

		bool GetAction(eActionCode action);
		bool GetActionDown(eActionCode action);
		bool GetActionUp(eActionCode action);

	public:
		void CreateDefaultBindings();
		void ProcessKeyBindings();
		void ExecuteBind(const KeyBindInfo& bindInfo);
		void ExecuteAction(eActionCode action);

		void BindKey(eKeyCode code, const KeyBindInfo& bindInfo);
		void UnbindKey(eKeyCode code);
		bool TryGetBindInfo(eKeyCode code, KeyBindInfo& outBindInfo) const;

	private:
		std::vector<Key> Keys;
		
		std::unordered_map<eKeyCode, KeyBindInfo, EnumClassHash> m_keyBindings;


	};

}

