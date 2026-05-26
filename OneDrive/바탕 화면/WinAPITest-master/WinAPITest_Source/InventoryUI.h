#pragma once
#include "CommonInclude.h"
#include "stbTexture.h"
#include "UI.h"
#include "InventoryUI_Info.h"

class Texture;
class stbD2DRenderer;

class InventoryUI : public UI
{
public:
	void Init() override;
	void Init_InventoryTab();
	void Init_InventoryButton();
	void Update() override;
	void Render(HDC hdc) override;
	void Render(stbD2DRenderer& renderer) override;
	

	void RenderBackGround(stbD2DRenderer& renderer);
	void RenderMenuButtons(stbD2DRenderer& renderer);
	void RenderButtons(stbD2DRenderer& renderer);
	void RenderSlotItem(stbD2DRenderer& renderer);

private:
	void CreateSlots();
	void UpdateSlots();
	void ClearSlots();

	void HandleLMouseClick(int mouseX, int mouseY);
	void HandleRMouseClick(int mouseX, int mouseY);
	bool HandleTabClick(int mouseX, int mouseY);
	bool HandleButtonClick(int mouseX, int mouseY);
	bool HandleInventoryClick(int mouseX, int mouseY);


	void UpdateButtonState(int mouseX, int mouseY);
	void UpdateSlotEnableState();
	void UpdateSingleButtonState(InventoryButton& button, int localX, int localY);

	bool IsPointInSlot(const InventorySlotUI& slot, int mouseX, int mouseY);
	bool IsPointInRect(const RECT& tabRect, int mouseX, int mouseY);
	void UpdateInventoryByType();
	int  GetClickedSlotIndex(int mouseX, int mouseY);
	

	void HandleMouseUp();
	void HandleDragging(int mouseX, int mouseY);

	void ExpandInventory();
	void ReduceInventory();

	stb::Texture* GetCurrentImg(InventoryButton& buttons);

private:
	struct Slot
	{
		RECT rect;
		int itemId = 0;
		int itemCount = 0;
	};

private:
	stb::Texture* m_background = nullptr;
	stb::Texture* m_fullBackground = nullptr;

	std::vector<Slot> mSlots;
	std::vector<InventorySlotUI> m_slots;

	InventoryType m_currentType;

	bool m_isDragging;
	bool m_isExpand = false;
	int m_dragOffsetX;
	int m_dragOffsetY;
private:
	int m_inventoryImgPosX = 300;
	int m_inventoryImgPosY = 100;

	static constexpr int m_inventoryClickWidth = 170;
	static constexpr int m_inventoryClickHeight = 30;

	static constexpr int m_fullInventoryClickWidth = 710;
	static constexpr int m_fullInventoryClickHeight = 30;
	
	static constexpr float m_slotStartX = 30;
	static constexpr float m_slotStartY = 100;
	static constexpr float m_slotWidth = 42;
	static constexpr float m_slotHeight = 40;
	static constexpr float m_slotgapX = 4.2;
	static constexpr float m_slotgapY = 6;
	
	static constexpr int m_slotCols = 4;
	static constexpr int m_fullSlotCols = 16;
	
	static constexpr int m_slotMaxCount = 128;

	RECT m_inventoryClickRect = { 300, 100, 470, 130 };

	static constexpr RECT m_equipTabRect	= { 10,30,45,52 };
	static constexpr RECT m_consumeTabRect	= { 47,30,82,52 };
	static constexpr RECT m_etcTabRect		= { 84,30,119,52 };
	static constexpr RECT m_setupTabRect	= { 121,30,156,52 };
	static constexpr RECT m_cashTabRect		= { 158, 30, 193, 52 };
	static constexpr RECT m_cosmeticTabRect = { 195, 30, 230, 52 };

	static constexpr RECT m_fullEquipTabRect	= {10,30,138,52 };
	static constexpr RECT m_fullConsumeTabRect	= {140, 30, 268, 52};
	static constexpr RECT m_fullEtcTabRect		= {270, 30, 398, 52};
	static constexpr RECT m_fullSetupTabRect	= {400 ,30, 528, 52};
	static constexpr RECT m_fullCashTabRect		= {530, 30, 658, 52};
	static constexpr RECT m_fullCosmeticTabRect = {660, 30, 788, 52};


	static constexpr RECT m_minimize_minButton		= { 170, 8, 189,27 };
	static constexpr RECT m_minimize_fullButton		= { 190, 8, 207,27 };
	static constexpr RECT m_minimize_closeButton	= { 208, 4, 234,31 };

	static constexpr RECT m_full_minButton		= {720, 8, 738, 27 };
	static constexpr RECT m_full_fullButton		= {740, 8, 758, 27 };
	static constexpr RECT m_full_closeButton	= {760, 4, 778, 27 };

private:

	InventoryButton m_fullButton;
	InventoryButton m_minButton;
	InventoryButton m_closeButton;

	std::unordered_map<int, InventoryTabButton> m_tabs;
	/*
	 int startX = 30;      // 인벤토리 배경 기준 첫 슬롯 x
   int startY = 100;      // 인벤토리 배경 기준 첫 슬롯 y
   int slotWidth = 42;
   int slotHeight = 40;
   int gapX = 4;
   int gapY = 6;
   int cols = 4;
   int maxSlotCount = 32;
	
	*/
};

