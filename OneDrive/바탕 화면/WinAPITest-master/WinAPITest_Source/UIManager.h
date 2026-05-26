#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"
<<<<<<< HEAD
#include "stbMath.h"
=======
#include "Trade_Info.h"
>>>>>>> main

class UI;
class InventoryUI;
class QuickSlotUI;
class HealthBarUI;
class stbD2DRenderer;
class TradeUI;
class ChatUI;

class UIManager : public stb::SingletonBase<UIManager>
{
public:
	UIManager();
	void Init();
	void Update();
	void Render(HDC hdc);
	void Render(stbD2DRenderer& renderer);

	void ToggleInventory();

public:
	InventoryUI* GetInventoryUI() { return m_inventoryUI; }
	QuickSlotUI* GetQuickSlotUI() { return m_quickslotUI; }
	//교환
	void OpenTradeUI();
	void CloseTradeUI();
	void ShowTradeRequestPopUp(const TradeRequestInfo& info); //신청 팝업

	//채팅
	void ToggleChatInput();
	bool IsInputFocused() const;
	void AppendInputChar(wchar_t ch);
	void HandleBackspace();
	void AppendChatMessage(const std::wstring& nick, const std::wstring& msg);
	void SubmitChatInput();

private:
	std::vector<UI*> mUIs;
	InventoryUI* m_inventoryUI = nullptr;
	QuickSlotUI* m_quickslotUI = nullptr;
	HealthBarUI* m_healthBarUI = nullptr;

	TradeUI* m_tradeUI = nullptr; //교환
	ChatUI* m_chatUI = nullptr; //채팅
};
