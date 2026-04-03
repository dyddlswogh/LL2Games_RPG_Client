#include "stbApplication.h"
#include "stbInput.h"
#include "stbTime.h"
#include "stbSceneManager.h"
#include "stbCollisionManager.h"


#define M_INPUT stb::SingletonBase<stb::Input>::getInstance()
#define M_TIME stb::SingletonBase<stb::Time>::getInstance()
#define M_SCENEMANAGER stb::SingletonBase<stb::SceneManager>::getInstance()
#define M_COLMANAGER stb::SingletonBase<stb::CollisionManager>::getInstance()


namespace stb
{
	Application::Application()
		: mHwnd(nullptr)
		, mWidth(0)
		, mHeight(0)
		//, Player(new GameObject())
		, mHdc(nullptr)
		, mBackHDC(nullptr)
		, mBackBuffer(nullptr)
	{
	}

	Application::~Application()
	{
	}

	void Application::Initialize(HWND hWnd, UINT width, UINT height)
	{
		adjustWindowRect(hWnd, width, height);
		createBuffer(width, height);


		M_INPUT->Initialize();
		M_TIME->Initialize();
		M_COLMANAGER->Initailzie();
		M_SCENEMANAGER->Initialize();
		
	}

	void Application::Run()
	{
		Update();
		Render();
	}

	void Application::Update()
	{
		M_INPUT->Update();
		M_TIME->Update();
		M_COLMANAGER->Update();
		M_SCENEMANAGER->Update();
	}

	void Application::Render()
	{
		ClearRenderTarget();


		M_TIME->Render(mBackHDC);
		M_COLMANAGER->Render(mBackHDC);
		M_SCENEMANAGER->Render(mBackHDC);
		copyDC(mBackHDC, mHdc);
	}

	void Application::Destroy()
	{

	}

	void Application::Release()
	{
	}

	void Application::adjustWindowRect(HWND hWnd, UINT width, UINT height)
	{
		mHwnd = hWnd;
		mHdc = GetDC(hWnd);

		RECT rect = { 0,0, width, height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;

		SetWindowPos(mHwnd, nullptr, 0, 0, mWidth, mHeight, 0);

		ShowWindow(mHwnd, true);

	}

	void Application::createBuffer(UINT width, UINT height)
	{
		mBackBuffer = CreateCompatibleBitmap(mHdc, width, height);

		mBackHDC = CreateCompatibleDC(mHdc);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(mBackHDC, mBackBuffer);

		DeleteObject(oldBitmap);
	}

	void stb::Application::copyDC(HDC source, HDC dest)
	{
		BitBlt(dest, 0, 0, mWidth, mHeight, source, 0, 0, SRCCOPY);
	}

	void stb::Application::ClearRenderTarget()
	{

		HBRUSH grayBrush = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(mBackHDC, grayBrush);


		Rectangle(mBackHDC, -1, -1, 1001, 1001);

		SelectObject(mBackHDC, oldBrush);
		DeleteObject(grayBrush);

	}
}
