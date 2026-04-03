#pragma once
#include "..\\WinAPITest_Source\\stbGameObject.h"

namespace stb
{
	class Luigi : public GameObject
	{
	public:
		Luigi();
		~Luigi();


		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

	private:


	};
}

