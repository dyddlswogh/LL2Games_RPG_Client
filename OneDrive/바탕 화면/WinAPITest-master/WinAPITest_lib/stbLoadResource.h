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

		/*인벤토리*/
		RESOURCEMANAGER->Load<Texture>(L"Inventory_normal", L"Resources\\UI\\Inventory\\Inventory_backgrnd.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full", L"Resources\\UI\\Inventory\\Inventory.FullBackgrnd.png");


		RESOURCEMANAGER->Load<Texture>(L"Inventory_equip_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_consume_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.1.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_etc_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.2.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_setup_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.3.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_cash_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.4.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_cosmetic_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.5.png");

		RESOURCEMANAGER->Load<Texture>(L"Inventory_equip_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_consume_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.1.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_etc_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.2.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_setup_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.3.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_cash_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.4.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_cosmetic_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.5.png");

		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_equip_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_consume_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.1.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_etc_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.2.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_setup_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.3.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_cash_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.4.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_cosmetic_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.5.png");

		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_equip_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_consume_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.1.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_etc_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.2.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_setup_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.3.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_cash_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.4.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_cosmetic_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.5.png");

		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_full.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_mouseOver", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_full.mouseOver.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_full_pressed", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_full.pressed.0.png");

		RESOURCEMANAGER->Load<Texture>(L"Inventory_min_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_min.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_min_mouseOver", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_min.mouseOver.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_min_pressed", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_min.pressed.0.png");


		RESOURCEMANAGER->Load<Texture>(L"Inventory_close_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_close.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_close_mouseOver", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_close.mouseOver.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Inventory_close_pressed", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_close.pressed.0.png");
		
		// 퀵슬롯 UI 리소스
		RESOURCEMANAGER->Load<Texture>(L"quickslot_background", L"Resources\\UI\\quickslot\\quickSlot.backgrnd.png");
		//RESOURCEMANAGER->Load<Texture>(L"Inventory_close_pressed", L"Resources\\UI\\quickslot\\quickSlot.backgrnd.png");
		// 아이템 리소스
		RESOURCEMANAGER->Load<Texture>(L"Item_Red_Potion", L"Resources\\Item\\Item_Red_Potion.png");
		RESOURCEMANAGER->Load<Texture>(L"Item_Blue_Potion", L"Resources\\Item\\Item_Blue_Potion.png");

		// 체력바 UI 리소스
		RESOURCEMANAGER->Load<Texture>(L"HealthBar_Background", L"Resources\\UI\\HealthBar\\main.status.normal.backgrnd.png");
		RESOURCEMANAGER->Load<Texture>(L"HealthBar_hpLayer", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.hp.layer_0.png");	
		RESOURCEMANAGER->Load<Texture>(L"HealthBar_mpLayer", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.mp.layer_0.png");
		RESOURCEMANAGER->Load<Texture>(L"HealthBar_guage", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.png");

		// 교환창
		RESOURCEMANAGER->Load<Texture>(L"Trade_normal", L"Resources\\UI\\ItemTrade\\itemTrade.backgrnd.png");
		RESOURCEMANAGER->Load<Texture>(L"Trade_full", L"Resources\\UI\\ItemTrade\\itemTrade.FullBackgrnd.png");

		RESOURCEMANAGER->Load<Texture>(L"Trade_button_confirm_normal", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.button_confirm.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Trade_button_trade_normal", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.button_trade.normal.0.png");
		RESOURCEMANAGER->Load<Texture>(L"Trade_button_trade_checked", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.button_trade.checked.0.png");
	

		// 몬스터 이미지 로드
		RESOURCEMANAGER->LoadMonsterTextures();

		stbD2DRenderer& renderer = stb::Application::getInstance()->GetRenderer();
		RESOURCEMANAGER->LoadAllD2D(renderer);
	}

}
