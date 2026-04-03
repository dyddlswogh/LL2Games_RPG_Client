#pragma once
#include "..//WinAPITest_Source//stbResourceManager.h"
#include "..//WinAPITest_Source//stbTexture.h"

#define RESOURCEMANAGER stb::SingletonBase<stb::ResourceManager>::getInstance()

namespace stb
{
	void LoadResource()
	{
		// 상대 경로: exe 기준 상위 폴더의 Resources (프로젝트 루트)
		// 또는 exe와 같은 폴더의 Resources
		RESOURCEMANAGER->Load<Texture>(L"Player", L"Resources\\Mario.png");
		RESOURCEMANAGER->Load<Texture>(L"Mario2", L"Resources\\Mario2.png");
		RESOURCEMANAGER->Load<Texture>(L"CharacterHead", L"Resources\\Character_Head.png");
		RESOURCEMANAGER->Load<Texture>(L"TwoHandSword", L"Resources\\two_hand_Sword.png");
		RESOURCEMANAGER->Load<Texture>(L"Knight", L"Resources\\Knight.png");
		RESOURCEMANAGER->Load<Texture>(L"DamonKnight", L"Resources\\damon_kinght.png");
		RESOURCEMANAGER->Load<Texture>(L"Sparta", L"Resources\\Sparta.png");
	}
}