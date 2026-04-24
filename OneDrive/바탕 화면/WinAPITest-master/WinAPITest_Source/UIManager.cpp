#include "UIManager.h"
#include "stbD2DRenderer.h"
#include "InventoryUI.h"
#include "QuickSlotUI.h"

UIManager::UIManager()
{
	m_inventoryUI = new InventoryUI();
	m_quickslotUI = new QuickSlotUI();
}

void UIManager::Init()
{
	m_inventoryUI->Init();
	m_quickslotUI->Init();


	mUIs.push_back(m_inventoryUI);
	mUIs.push_back(m_quickslotUI);


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
