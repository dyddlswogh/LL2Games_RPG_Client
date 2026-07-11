#include "stbD2DRenderer.h"
#include "stbTexture.h"
#include "stbResourceManager.h"
#include <nlohmann/json.hpp>
#include "stbAudioClip.h"

stb::ResourceManager::ResourceManager()
{
}

stb::ResourceManager::~ResourceManager()
{
}

void stb::ResourceManager::LoadAllD2D(stbD2DRenderer& renderer)
{
    for (auto& pair : mResources)
    {
        stb::Texture* tex = dynamic_cast<stb::Texture*>(pair.second);
        if (tex) tex->LoadD2D(renderer);
    }
}

void stb::ResourceManager::LoadMonsterTextures()
{
	std::filesystem::path root = L"Resources\\Monster";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}
			

		if (entry.path().extension() != ".png")
		{
			continue;
		}
			

		std::filesystem::path relative = std::filesystem::relative(entry.path(), L"Resources");

		std::wstring key = relative.replace_extension(L"").generic_wstring();

		OutputDebugStringW(key.c_str());
		OutputDebugStringA("\n");
		Load<Texture>(key, entry.path().wstring());
	}
}

void stb::ResourceManager::LoadPlayerTextures()
{
	std::filesystem::path root = L"Resources\\Character";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}


		if (entry.path().extension() != ".png")
		{
			continue;
		}


		std::filesystem::path relative = std::filesystem::relative(entry.path(), L"Resources");

		std::wstring key = relative.replace_extension(L"").generic_wstring();

		OutputDebugStringW(key.c_str());
		OutputDebugStringA("\n");
		Load<Texture>(key, entry.path().wstring());
	}
}

void stb::ResourceManager::LoadItemTextures()
{
	std::filesystem::path root = L"Resources\\Item";

	for (const auto& entry : std::filesystem::recursive_directory_iterator(root))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}

			
		if (entry.path().extension() != ".png")
		{
			continue;
		}

		std::wstring key = entry.path().stem().wstring();;

		OutputDebugStringW(key.c_str());
		OutputDebugStringA("\n");
		Load<Texture>(key, entry.path().wstring());
	}
}

void stb::ResourceManager::LoadHealthBarTextures()
{
	Load<Texture>(L"HealthBar_Background", L"Resources\\UI\\HealthBar\\main.status.normal.backgrnd.png");
	Load<Texture>(L"HealthBar_hpLayer", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.hp.layer_0.png");
	Load<Texture>(L"HealthBar_mpLayer", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.mp.layer_0.png");
	Load<Texture>(L"HealthBar_guage", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.png");

	Load<Texture>(L"HealthBar_0", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.0.png");
	Load<Texture>(L"HealthBar_1", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.1.png");
	Load<Texture>(L"HealthBar_2", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.2.png");
	Load<Texture>(L"HealthBar_3", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.3.png");
	Load<Texture>(L"HealthBar_4", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.4.png");
	Load<Texture>(L"HealthBar_5", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.5.png");
	Load<Texture>(L"HealthBar_6", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.6.png");
	Load<Texture>(L"HealthBar_7", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.7.png");
	Load<Texture>(L"HealthBar_8", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.8.png");
	Load<Texture>(L"HealthBar_9", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.9.png");
	Load<Texture>(L"HealthBar_/", L"Resources\\UI\\HealthBar\\main.status.normal.gauge.number.png");
}

void stb::ResourceManager::LoadExpTextures()
{
	Load<Texture>(L"1228_expBar_backLayer", L"Resources\\UI\\Exp\\main.expBar.1228.layer_back.png");
	Load<Texture>(L"1228_expBar_gaugeLayer", L"Resources\\UI\\Exp\\main.expBar.1228.layer_gauge.png");
	Load<Texture>(L"1280_expBar_backLayer", L"Resources\\UI\\Exp\\main.expBar.1280.layer_back.png");
	Load<Texture>(L"1280_expBar_gaugeLayer", L"Resources\\UI\\Exp\\main.expBar.1280.layer_gauge.png");
	Load<Texture>(L"1366_expBar_backLayer", L"Resources\\UI\\Exp\\main.expBar.1366.layer_back.png");
	Load<Texture>(L"1366_expBar_gaugeLayer", L"Resources\\UI\\Exp\\main.expBar.1366.layer_gauge.png");
	Load<Texture>(L"1920_expBar_backLayer", L"Resources\\UI\\Exp\\main.expBar.1920.layer_back.png");
	Load<Texture>(L"1920_expBar_gaugeLayer", L"Resources\\UI\\Exp\\main.expBar.1920.layer_gauge.png");


}


void stb::ResourceManager::LoadLevelTextures()
{
	Load<Texture>(L"Level_Background",			L"Resources\\UI\\Level\\main.namePlate.backgrnd.png");
	Load<Texture>(L"Level_lvLayer",				L"Resources\\UI\\Level\\main.namePlate.layer_lv.png");
	Load<Texture>(L"Level_0",					L"Resources\\UI\\Level\\main.namePlate.levelNum.0.png");
	Load<Texture>(L"Level_1",					L"Resources\\UI\\Level\\main.namePlate.levelNum.1.png");
	Load<Texture>(L"Level_2",					L"Resources\\UI\\Level\\main.namePlate.levelNum.2.png");
	Load<Texture>(L"Level_3",					L"Resources\\UI\\Level\\main.namePlate.levelNum.3.png");
	Load<Texture>(L"Level_4",					L"Resources\\UI\\Level\\main.namePlate.levelNum.4.png");
	Load<Texture>(L"Level_5",					L"Resources\\UI\\Level\\main.namePlate.levelNum.5.png");
	Load<Texture>(L"Level_6",					L"Resources\\UI\\Level\\main.namePlate.levelNum.6.png");
	Load<Texture>(L"Level_7",					L"Resources\\UI\\Level\\main.namePlate.levelNum.7.png");
	Load<Texture>(L"Level_8",					L"Resources\\UI\\Level\\main.namePlate.levelNum.8.png");
	Load<Texture>(L"Level_9",					L"Resources\\UI\\Level\\main.namePlate.levelNum.9.png");
}

void stb::ResourceManager::LoadInventoryTextures()
{
	Load<Texture>(L"Inventory_normal", L"Resources\\UI\\Inventory\\Inventory_backgrnd.png");
	Load<Texture>(L"Inventory_full", L"Resources\\UI\\Inventory\\Inventory.FullBackgrnd.png");

	Load<Texture>(L"Inventory_equip_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.0.png");
	Load<Texture>(L"Inventory_consume_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.1.png");
	Load<Texture>(L"Inventory_etc_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.2.png");
	Load<Texture>(L"Inventory_setup_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.3.png");
	Load<Texture>(L"Inventory_cash_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.4.png");
	Load<Texture>(L"Inventory_cosmetic_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.normal.5.png");

	Load<Texture>(L"Inventory_equip_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.0.png");
	Load<Texture>(L"Inventory_consume_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.1.png");
	Load<Texture>(L"Inventory_etc_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.2.png");
	Load<Texture>(L"Inventory_setup_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.3.png");
	Load<Texture>(L"Inventory_cash_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.4.png");
	Load<Texture>(L"Inventory_cosmetic_selected", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.tab-category.selected.5.png");

	Load<Texture>(L"Inventory_full_equip_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.0.png");
	Load<Texture>(L"Inventory_full_consume_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.1.png");
	Load<Texture>(L"Inventory_full_etc_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.2.png");
	Load<Texture>(L"Inventory_full_setup_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.3.png");
	Load<Texture>(L"Inventory_full_cash_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.4.png");
	Load<Texture>(L"Inventory_full_cosmetic_normal", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.normal.5.png");

	Load<Texture>(L"Inventory_full_equip_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.0.png");
	Load<Texture>(L"Inventory_full_consume_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.1.png");
	Load<Texture>(L"Inventory_full_etc_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.2.png");
	Load<Texture>(L"Inventory_full_setup_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.3.png");
	Load<Texture>(L"Inventory_full_cash_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.4.png");
	Load<Texture>(L"Inventory_full_cosmetic_selected", L"Resources\\UI\\Inventory\\Inventory.FullAutoBuild.tab-category.selected.5.png");

	Load<Texture>(L"Inventory_full_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_full.normal.0.png");
	Load<Texture>(L"Inventory_full_mouseOver", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_full.mouseOver.0.png");
	Load<Texture>(L"Inventory_full_pressed", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_full.pressed.0.png");

	Load<Texture>(L"Inventory_min_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_min.normal.0.png");
	Load<Texture>(L"Inventory_min_mouseOver", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_min.mouseOver.0.png");
	Load<Texture>(L"Inventory_min_pressed", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_min.pressed.0.png");


	Load<Texture>(L"Inventory_close_normal", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_close.normal.0.png");
	Load<Texture>(L"Inventory_close_mouseOver", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_close.mouseOver.0.png");
	Load<Texture>(L"Inventory_close_pressed", L"Resources\\UI\\Inventory\\Inventory.AutoBuild.button_close.pressed.0.png");

}

void stb::ResourceManager::LoadTradeTextures()
{
	Load<Texture>(L"Trade_normal", L"Resources\\UI\\ItemTrade\\itemTrade.backgrnd.png");
	Load<Texture>(L"Trade_full", L"Resources\\UI\\ItemTrade\\itemTrade.FullBackgrnd.png");

	Load<Texture>(L"Trade_button_confirm_normal", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.button_confirm.normal.0.png");
	Load<Texture>(L"Trade_button_trade_normal", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.button_trade.normal.0.png");
	Load<Texture>(L"Trade_button_trade_checked", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.button_trade.checked.0.png");

	Load<Texture>(L"Trade_layer_confirm_me", L"Resources\\UI\\ItemTrade\\itemTrade.AutoBuild.layer_confirmMe.png");
}

void stb::ResourceManager::LoadBGMAudioClips()
{
	Load<AudioClip>(L"BGM_Forest_ground_1", L"Resources\\BGM\\forest\\041415calmbgm.mp3");
}


void stb::ResourceManager::LoadMapTextures()
{
	Load<Texture>(L"Forest_ground_1", L"Resources\\Background\\forest\\forest_ground_1.png");
}

void stb::ResourceManager::LoadQuickSlotTextures()
{
	Load<Texture>(L"quickslot_background", L"Resources\\UI\\quickslot\\quickSlot.backgrnd.png");
}