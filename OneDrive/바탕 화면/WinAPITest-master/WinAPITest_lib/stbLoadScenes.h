#pragma once

#include "..\\WinAPITest_Source\\stbSceneManager.h"

#include "stbPlayScene.h"

#define SCENEMANAGER stb::SingletonBase<stb::SceneManager>::getInstance()

namespace stb
{
	void LoadScene()
	{
		SCENEMANAGER->CreateScene<PlayScene>(L"PlayScene");


		SCENEMANAGER->LoadScene(L"PlayScene");

	}



}

