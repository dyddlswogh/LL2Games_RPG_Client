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
		RESOURCEMANAGER->Load<Texture>(L"Inventory", L"Resources\\UI\\Inventory\\Inventory_backgrnd.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_equip_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_consume_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.1.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_etc_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.2.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_setup_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.3.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_equip_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_consume_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.1.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_etc_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.2.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_setup_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.3.png");

		RESOURCEMANAGER->Load<Texture>(L"Item_Red_Potion", L"Resources\\Item\\Item_Red_Potion.png");

		stbD2DRenderer& renderer = stb::Application::getInstance()->GetRenderer();
		RESOURCEMANAGER->LoadAllD2D(renderer);
	}
}
