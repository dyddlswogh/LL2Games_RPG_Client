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
class ChatUI;
class TradeRequestUI;

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
	void ToggleTradeUI(); //test
	void OpenTradeUI();
	void OpenReqTradeUI();
	void CloseReqTradeUI();
	void AppendInputChar_Trade(wchar_t ch);
	void HandleBackspace_Trade();
	void KeyDownTrade(WPARAM key);
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
	TradeRequestUI* m_tradeReqUI = nullptr; //교환
	ChatUI* m_chatUI = nullptr; //채팅
};
