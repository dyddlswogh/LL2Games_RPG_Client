#include "UIManager.h"
#include "stbD2DRenderer.h"
#include "InventoryUI.h"

void UIManager::Init()
{
	mInventoryUI = new InventoryUI();
	mInventoryUI->Init();

	mUIs.push_back(mInventoryUI);
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
	if (mInventoryUI != nullptr)
		mInventoryUI->Toggle();
}
