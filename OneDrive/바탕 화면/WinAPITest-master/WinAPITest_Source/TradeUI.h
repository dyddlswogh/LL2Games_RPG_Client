#pragma once
#include "UI.h"
#include "Trade_Info.h"

class stbD2DRenderer;

class TradeUI : public UI
{
public:
	void Init() override;
	void Update() override;
	void Render(HDC hdc) override;
	void Render(stbD2DRenderer& renderer) override;

private:
	//내 아이템 슬롯 영역(왼쪽 패널)
	void RenderMySlots(stbD2DRenderer& renderer);
	//상대 아이템 슬롯 영역
	void RenderTargetSlots(stbD2DRenderer& renderer);
	//확정/취소 버튼
	void RenderButtons(stbD2DRenderer& renderer);

	void HandleLMouseClick(int mouseX, int mouseY);
	int GetClickedMySlotIndex(int mouseX, int mouseY);

private:
	int m_posX = 200;
	int m_posY = 150;

	//슬롯 레이아웃 (InventoryUI와 동일한 방식)
	static constexpr int SLOT_COLS = 4;
	static constexpr int SLOT_ROWS = 4;
	static constexpr int SLOT_W = 32;
	static constexpr int SLOT_H = 32;
	static constexpr int SLOT_GAP_X = 2;
	static constexpr int SLOT_GAP_Y = 2;
	
};