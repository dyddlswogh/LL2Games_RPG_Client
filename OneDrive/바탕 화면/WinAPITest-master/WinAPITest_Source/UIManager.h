#pragma once
#include "CommonInclude.h"
#include "stbSingletonBase.h"

class UI;
class InventoryUI;
class stbD2DRenderer;

class UIManager : public stb::SingletonBase<UIManager>
{
public:
	void Init();
	void Update();
	void Render(HDC hdc);
	void Render(stbD2DRenderer& renderer);

	void ToggleInventory();

private:
	std::vector<UI*> mUIs;
	InventoryUI* mInventoryUI = nullptr;
};
