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
	private:
		std::vector<Key> Keys;


	};

}

