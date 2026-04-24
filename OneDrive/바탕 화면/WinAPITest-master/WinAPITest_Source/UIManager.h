#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"

class UI;
class InventoryUI;
class QuickSlotUI;
class stbD2DRenderer;

class UIManager : public stb::SingletonBase<UIManager>
{
public:
	UIManager();
	void Init();
	void Update();
	void Render(HDC hdc);
	void Render(stbD2DRenderer& renderer);

	void ToggleInventory();

private:
	std::vector<UI*> mUIs;
	InventoryUI* m_inventoryUI = nullptr;
	QuickSlotUI* m_quickslotUI = nullptr;
};
