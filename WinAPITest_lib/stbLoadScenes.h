#pragma once

#include "..\\WinAPITest_Source\\stbSceneManager.h"

#include "stbPlayScene.h"
#include "stbChatScene.h"

#define SCENEMANAGER stb::SingletonBase<stb::SceneManager>::getInstance()

namespace stb
{
	void LoadScene()
	{
		SCENEMANAGER->CreateScene<PlayScene>(L"PlayScene");
		SCENEMANAGER->CreateScene<ChatScene>(L"ChatScene");


		SCENEMANAGER->LoadScene(L"PlayScene");
		SCENEMANAGER->SetOverlayScene(L"ChatScene");
	}

}

