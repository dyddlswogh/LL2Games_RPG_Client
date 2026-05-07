#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"
#include "Trade_Info.h"

class UI;
class InventoryUI;
class QuickSlotUI;
class HealthBarUI;
class stbD2DRenderer;
class TradeUI;


class UIManager : public stb::SingletonBase<UIManager>
{
public:
	UIManager();
	void Init();
	void Update();
	void Render(HDC hdc);
	void Render(stbD2DRenderer& renderer);

	void ToggleInventory();

	//교환
	void OpenTradeUI();
	void CloseTradeUI();
	void ShowTradeRequestPopUp(const TradeRequestInfo& info); //신청 팝업

private:
	std::vector<UI*> mUIs;
	InventoryUI* m_inventoryUI = nullptr;
	QuickSlotUI* m_quickslotUI = nullptr;
	HealthBarUI* m_healthBarUI = nullptr;

	//교환
	TradeUI* m_tradeUI = nullptr;
};
