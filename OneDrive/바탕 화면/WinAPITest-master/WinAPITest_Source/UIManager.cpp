#include "UIManager.h"
#include "stbD2DRenderer.h"
#include "InventoryUI.h"
#include "QuickSlotUI.h"
#include "HealthBarUI.h"
#include "TradeUI.h"
#include "ChatUI.h"
#include "ExpBarUI.h"
#include "LevelUI.h"

UIManager::UIManager()
{
	m_inventoryUI = new InventoryUI();
	m_quickslotUI = new QuickSlotUI();
	m_healthBarUI = new HealthBarUI();
	m_expBarUI = new ExpBarUI();
	m_levelUI = new LevelUI();
	m_tradeUI = new TradeUI();
	m_chatUI = new ChatUI();
	
}

void UIManager::Init()
{
	m_inventoryUI->Init();
	m_quickslotUI->Init();
	m_healthBarUI->Init();
	m_expBarUI->Init();
	m_levelUI->Init();
	m_tradeUI->Init();
	m_chatUI->Init();
	

	mUIs.push_back(m_inventoryUI);
	mUIs.push_back(m_quickslotUI);
	mUIs.push_back(m_healthBarUI);
	mUIs.push_back(m_expBarUI);
	mUIs.push_back(m_levelUI);
	mUIs.push_back(m_tradeUI);
	mUIs.push_back(m_chatUI);
	


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
		if (ui == nullptr || ui->IsActive() == false)
			continue;

		ui->Render(renderer);
	}
}

void UIManager::ToggleInventory()
{
	if (m_inventoryUI != nullptr)
		m_inventoryUI->Toggle();
}


void UIManager::OpenTradeUI()
{
	m_tradeUI->SetActivce(true);
}

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
