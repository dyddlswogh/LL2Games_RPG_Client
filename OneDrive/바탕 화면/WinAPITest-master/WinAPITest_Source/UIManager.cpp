#include "UIManager.h"
#include "stbD2DRenderer.h"
#include "InventoryUI.h"
#include "QuickSlotUI.h"
#include "HealthBarUI.h"
#include "TradeUI.h"
#include "ChatUI.h"
#include "TradeRequestUI.h"

UIManager::UIManager()
{
	m_inventoryUI = new InventoryUI();
	m_quickslotUI = new QuickSlotUI();
	m_healthBarUI = new HealthBarUI();
	m_tradeUI = new TradeUI();
	m_chatUI = new ChatUI();
	m_tradeReqUI = new TradeRequestUI();
}

void UIManager::Init()
{
	m_inventoryUI->Init();
	m_quickslotUI->Init();
	m_healthBarUI->Init();
	m_tradeUI->Init();
	m_chatUI->Init();
	m_tradeReqUI->Init();

	mUIs.push_back(m_inventoryUI);
	mUIs.push_back(m_quickslotUI);
	mUIs.push_back(m_healthBarUI);
	mUIs.push_back(m_tradeUI);
	mUIs.push_back(m_chatUI);
	mUIs.push_back(m_tradeReqUI);


	char msg[128];
	sprintf_s(msg, "UI count = %zu\n", mUIs.size());
	OutputDebugStringA(msg);
}

void UIManager::Update()
{
	for (UI* ui : mUIs)
	{
		if (ui == nullptr || ui->IsActive() == false)
			continue;

		ui->Update();
	}
}

void UIManager::Render(HDC hdc)
{

}

void UIManager::Render(stbD2DRenderer& renderer)
{

	for (UI* ui : mUIs)
	{
		//if (ui == nullptr || ui->IsActive() == false)
		if (ui == nullptr)
			continue;

		ui->Render(renderer);
	}
}

void UIManager::ToggleInventory()
{
	if (m_inventoryUI != nullptr)
		m_inventoryUI->Toggle();
}

#if 1 //교환
void UIManager::OpenTradeUI(const std::string& targetId, const std::string& targetName)
{
	m_tradeUI->SetActivce(true);
	m_tradeUI->StartTrade(targetId, targetName);
}
void UIManager::CloseTradeUI()
{
	m_tradeUI->CloseTradeUI();
}

void UIManager::ShowCancelPopUp()
{
	m_tradeUI->OnCancelPopUp();
}

void UIManager::ShowSuccessPopUp()
{
	m_tradeUI->OnSuccessPopUp();
}

void UIManager::TradeReadyTarget()
{
	m_tradeUI->OnReady();
}

void UIManager::ToggleTradeUI()
{
	if (m_tradeUI != nullptr)
		m_tradeUI->Toggle();
}

void UIManager::OnTradeAddItem(const TradeSlotInfo& tradeSlotInfo)
{
	if (m_tradeUI) m_tradeUI->OnTargetAddItem(tradeSlotInfo);
}

void UIManager::OpenReqTradeUI()
{
	if (m_tradeReqUI != nullptr)
		m_tradeReqUI->SetActivce(true);
}

void UIManager::CloseReqTradeUI()
{
	if (m_tradeReqUI != nullptr)
		m_tradeReqUI->CloseWindow();
		//m_tradeReqUI->SetActivce(false);
}

void UIManager::AppendInputChar_Trade(wchar_t ch)
{
	if (m_tradeReqUI) m_tradeReqUI->OnChar(ch);
}

void UIManager::HandleBackspace_Trade()
{
	if (m_tradeReqUI) m_tradeReqUI->Backspace();
}

void UIManager::KeyDownTrade(WPARAM key)
{
	if (m_tradeReqUI) m_tradeReqUI->OnKeyDown(key);
}

void UIManager::ShowTradeRequestPopUp(const TradeRequestInfo& info)
{
	if (m_tradeReqUI) m_tradeReqUI->OnPopUp(info);
}

#endif //교환

#if 1 //채팅
void UIManager::ToggleChatInput()
{
	if (m_chatUI) m_chatUI->ToggleInputFocus();
}
bool UIManager::IsInputFocused() const
{
	return m_chatUI && m_chatUI->IsInputActive();
}
void UIManager::AppendInputChar(wchar_t ch)
{
	if (m_chatUI) m_chatUI->AppendChar(ch);
}
void UIManager::HandleBackspace()
{
	if (m_chatUI) m_chatUI->Backspace();
}
void UIManager::AppendChatMessage(const std::wstring& nick, const std::wstring& msg)
{
	if (m_chatUI) m_chatUI->AddMessage(nick, msg);
}

void UIManager::SubmitChatInput()
{
	if (m_chatUI) m_chatUI->SubmitInput();
}
#endif //채팅