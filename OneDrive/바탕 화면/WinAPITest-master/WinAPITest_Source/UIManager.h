#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"
#include "stbMath.h"

class UI;
class InventoryUI;
class QuickSlotUI;
class HealthBarUI;
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

public:
	InventoryUI* GetInventoryUI() { return m_inventoryUI; }
	QuickSlotUI* GetQuickSlotUI() { return m_quickslotUI; }
private:
	std::vector<UI*> mUIs;
	InventoryUI* m_inventoryUI = nullptr;
	QuickSlotUI* m_quickslotUI = nullptr;
	HealthBarUI* m_healthBarUI = nullptr;

};
