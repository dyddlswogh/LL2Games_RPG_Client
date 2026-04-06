#pragma once
#include "..\\WinAPITest_Source\\stbResourceManager.h"
#include "..\\WinAPITest_Source\\stbTexture.h"
#include "..\\WinAPITest_Source\\stbApplication.h"
#include "..\\WinAPITest_Source\\stbD2DRenderer.h"

#define RESOURCEMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()

namespace stb
{
	void LoadResource()
	{
		RESOURCEMANAGER->Load<Texture>(L"Player", L"Resources\\Mario.png");
		RESOURCEMANAGER->Load<Texture>(L"Mario2", L"Resources\\Mario2.png");
		RESOURCEMANAGER->Load<Texture>(L"CharacterHead", L"Resources\\Character_Head.png");
		RESOURCEMANAGER->Load<Texture>(L"TwoHandSword", L"Resources\\two_hand_Sword.png");
		RESOURCEMANAGER->Load<Texture>(L"Knight", L"Resources\\Knight.png");
		RESOURCEMANAGER->Load<Texture>(L"DamonKnight", L"Resources\\damon_kinght.png");
		RESOURCEMANAGER->Load<Texture>(L"Sparta", L"Resources\\Sparta.png");

		stbD2DRenderer& renderer = stb::Application::getInstance()->GetRenderer();
		RESOURCEMANAGER->LoadAllD2D(renderer);
	}
}
