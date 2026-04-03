#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"

namespace stb
{
	class Application : public SingletonBase<Application>
	{
	public:
		Application();
		~Application();

		// 윈도우의 사이즈와 핸들을 받아온다.
		void Initialize(HWND hWnd, UINT width, UINT height);
		void Run();
		void Update();
		void Render();
		void Destroy();
		void Release();

		HWND GetHWND() { return mHwnd; }
		HDC GetHDC() { return mHdc; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

	private:
		void ClearRenderTarget();
		void adjustWindowRect(HWND hWnd, UINT width, UINT height);
		void createBuffer(UINT width, UINT height);
		void copyDC(HDC source, HDC dest);
		// 이것저것 시도하다가 안되면 보기 
		// 라이브러리 폴더에는 오브젝트 파일이 있었다.

	private:
		HWND mHwnd;
		HDC  mHdc;
		HDC  mBackHDC;
		HBITMAP mBackBuffer;
		UINT mWidth;
		UINT mHeight;
		//class GameObject* Player;


		// 더블 버퍼링 구현 
		/*
		윈도우의 크기 만큼 bitmap 만들어서 Render 해주고 원래 HDC에 복사
		윈도우의 크기를 미리 가지고 온다.


		*/


	};
}

