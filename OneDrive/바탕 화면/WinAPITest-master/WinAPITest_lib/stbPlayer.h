#pragma once
#include "..\\WinAPITest_Source\\stbGameObject.h"



namespace stb
{
	class Player : public GameObject
	{
	public:
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
	private:




	};

}

